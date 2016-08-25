//
// This program can be freely used in an academic environment
// ONLY for research purposes, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    an acknowledgment in the product documentation is required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//
// Any other use is strictly prohibited by the author, without an explicit 
// permission.
//
// Note that this program uses the LEDA library, which is NOT free. For more 
// details visit Algorithmic Solutions at http://www.algorithmic-solutions.com/
// There is also a free version of LEDA 6.0 or newer.
//
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// ! Any commercial use of this software is strictly !
// ! prohibited without explicit permission by the   !
// ! author.                                         !
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Copyright (C) 2004-2008 - Dimitrios Michail <dimitrios.michail@gmail.com>
//

/*! \file dsigned.h
 *  \brief Algorithms for computing non-orthogonal shortest paths in directed graphs
 *         modulo some number.
 *  \sa mcb::spvecfp
 */

#ifndef DSIGNED_H
#define DSIGNED_H

#include <LEP/mcb/config.h>

#ifdef LEDA_GE_V5
#include <LEDA/graph/graph.h>
#include <LEDA/graph/edge_array.h>
#include <LEDA/core/array.h>
#include <LEDA/core/list.h>
#include <LEDA/core/p_queue.h>
#include <LEDA/core/random_source.h>
#include <LEDA/core/sortseq.h>
#include <LEDA/core/tuple.h>
#include <LEDA/numbers/integer.h>
#else
#include <LEDA/graph.h>
#include <LEDA/array.h>
#include <LEDA/edge_array.h>
#include <LEDA/list.h>
#include <LEDA/integer.h>
#include <LEDA/p_queue.h>
#include <LEDA/random_source.h>
#include <LEDA/sortseq.h>
#include <LEDA/tuple.h>
#endif

#include <LEP/mcb/edge_num.h>
#include <LEP/mcb/fp.h>
#include <LEP/mcb/spvecfp.h>
#include <LEP/mcb/arithm.h>

namespace mcb { 

#if defined(LEDA_NAMESPACE)
    using leda::graph;
    using leda::edge;
    using leda::edge_array;
    using leda::node;
    using leda::node_array;
    using leda::array;
    using leda::list;
    using leda::node_list;
    using leda::integer;
    using leda::random_source;
    using leda::p_queue;
    using leda::pq_item;
    using leda::sortseq;
    using leda::seq_item;
    using leda::three_tuple;
    using leda::four_tuple;
#endif


    // defines for SSitem
#define mcb_revi() first()
#define mcb_dist() second()
#define mcb_num_edges() third()
#define mcb_pred() fourth()

    // defines for preditem
#define mcb_edge() first()
#define mcb_other_node_index() second()
#define mcb_other_node() third()


    // T = weight
    // PT = type of prime p
    template<class T, class PT> 
        class dirsp { 

            typedef three_tuple< edge, PT, node > preditem;
            typedef four_tuple< pq_item, T, indextype, preditem > SSitem;
            typedef two_tuple< node, PT > pnode;

            public:


            // constructor
            dirsp( const graph& g, const edge_array<T>& length , 
                    const PT& pin, const edge_num& en ) : 
                n(g.number_of_nodes()), 
                m(g.number_of_edges()), 
                p( pin ), 
                G( g ),
                len ( length ),
                enumb( en ),
                SS( mcb::compare ),
                cycle ( m ),
                nodeid( g ), 
                marked( g, 0 ),
                Xe( g )
            {   
                // give a numbering to the nodes
                indextype i = 0; 
                node v;
                forall_nodes( v, g ) { 
                    nodeid[ v ] = i;
                    i++;
                }
            }

            // reinitialize with different p
            // used for execution on same graph but different p
            // in order to reduce error probability
            void reinit( const PT& pin ) { 
                p = pin;
            }

            // destructor
            ~dirsp() { 
            }


            // compute shortest cycle
            spvecfp get_shortest_cycle( const spvecfp& X , T& weight ) { 

                // create X, on edge index
                fill_X( X );

                // run shortest paths
                weight = run_shortest_paths();

                // construct sparse vector from cycle
                spvecfp ret( p );
                for( indextype i = 0; i < m; i++ ) { 
                    if ( cycle[ i ] != 0 ) { 
                        ret.append( i, cycle[i] );      
                    }
                }

                return ret;
            }

            private:

            // get X on edge index, by spvecfp
            void fill_X( const spvecfp& X ) { 
                edge e;
                forall_edges( e, G ) 
                    Xe[ e ] = 0;

                list_item li = X.first();
                while( li != nil ) { 
                    Xe[ enumb( X.index( li ) ) ] = X.inf( li ) ;
                    li = X.succ( li );
                }
            }


            // record a cycle, by traversing pred array
            void record_cycle( const PT& t, array< etype >& cyc ) { 
                cyc.init( 0 );

                PT cur = t;
                seq_item sicur = SS.lookup( cur );
                node u;
                edge e;
                while( SS[ sicur ].mcb_pred().mcb_edge() != nil ) { 

                    u = SS[ sicur ].mcb_pred().mcb_other_node();
                    e = SS[ sicur ].mcb_pred().mcb_edge();
                    if ( u == G.target( e ) ) { // give + direction
                        cyc[ enumb( e ) ] = 1;
                    }
                    else { // give - direction
                        cyc[ enumb( e ) ] = -1;
                    }

                    cur = SS[ sicur ].mcb_pred().mcb_other_node_index();
                    sicur = SS.lookup( cur );
                }
            }


            // get the index of a node in the reverse index of the PQ
            inline PT get_revi_index( const node& v, const PT& level ) { 
                return level * n + nodeid[ v ];
            }


            // init used data structures
            void init_used_dijkstra() { 
                // init marked
                while( marked_touched.empty() == false ) 
                    marked[ marked_touched.pop() ] = 0;

                // init all nodes reached by SP
                SS.clear();

                // clear priority queue
                PQ.clear();
            }


            // execute the shortest paths
            T run_shortest_paths( ) { 

                T minall = T(0);
                indextype minall_num_edges = 0;
                bool is_minall_inf = true;


                node v; 
                forall_nodes( v, G ) { // do n shortest paths

                    T min = T(0);
                    indextype min_num_edges = 0;
                    bool is_min_inf = true;

                    // add source vertex to PQ
                    PT vi = get_revi_index( v, 0 );
                    SS.insert( vi , 
                            SSitem( PQ.insert( T(0) , pnode( v, 0 ) ),
                                T( 0 ),
                                0,
                                preditem( nil, 0 , nil )
                                ) 
                            );


                    // do shortest path
                    pq_item pqi;
                    node u, w; // nodes
                    PT ul, wl; // levels
                    PT uindex, windex; // index on arrays
                    seq_item siu, siw;
                    edge e;
                    while( PQ.empty() == false ) { 
                        // extract min
                        pqi = PQ.find_min(); 
                        u = PQ[ pqi ].first(); // get vertex
                        ul = PQ[ pqi ].second(); // get level
                        PQ.del_item( pqi );

                        // find index of u in level graph
                        uindex = get_revi_index( u, ul );
                        siu = SS.lookup( uindex );

                        // check if we are done
                        if ( u == v && ul != 0 ) { 
                            min = SS[ siu ].mcb_dist();
                            min_num_edges = SS[ siu ].mcb_num_edges();
                            is_min_inf = false;
                            break;
                        }


                        // check if vertex is too far
                        if ( marked[ u ] >= 2 ) continue;

                        // increment marked
                        marked[ u ] = marked[ u ] + 1;
                        if ( marked[ u ] == 1 ) marked_touched.append( u );



                        // find neighbors
                        forall_inout_edges( e, u ) { 

                            w = G.opposite( e, u );

                            // find level of w
                            if ( w == G.target( e ) ) {  // e = u -> w
                                wl = ( ul + Xe[ e ] ) % p;
                            }
                            else { // e = w -> u
                                wl = ( ul - Xe[ e ] ) % p;
                            }
                            while ( wl < 0 ) wl += p; // [-i]_p = [p-i]_p


                            // find index in level graph
                            windex = get_revi_index( w, wl );
                            siw = SS.lookup( windex );

                            // now update if necessary
                            T c = SS[ siu ].mcb_dist() + len [ e ];
                            if ( siw == nil ) { 
                                // don't insert if more than current minimum
                                if ( is_minall_inf == false && c > minall )
                                    continue;
                                // insert ( for now, with some info undetermined )
                                siw = SS.insert( windex, 
                                        SSitem( PQ.insert( c, pnode( w , wl ) ), 
                                            c , 
                                            0 , 
                                            preditem( nil, 0, nil ) ) );   
                            }
                            else if ( c < SS[ siw ].mcb_dist() ) { 
                                PQ.decrease_p( SS[ siw ].mcb_revi() , c );
                            }
                            else continue;

                            // record changes
                            SS[ siw ].mcb_dist() = c;
                            SS[ siw ].mcb_num_edges() = SS[ siu ].mcb_num_edges() + 1;
                            SS[ siw ].mcb_pred().mcb_edge() = e;
                            SS[ siw ].mcb_pred().mcb_other_node_index() = uindex;
                            SS[ siw ].mcb_pred().mcb_other_node() = u;
                        }

                    }



                    // possible record path
                    if ( is_min_inf == false ) {
                        if ( ( is_minall_inf == true ) ||
                                ( min < minall ) || 
                                ( ( min == minall ) && ( min_num_edges < minall_num_edges ) ) 
                           ) { 
                            // record new path as better either if it has less weight
                            // or if the weight is equal and it has less number of edges

                            // record path to a cycle
                            record_cycle( get_revi_index( u, ul ), cycle );
                            minall = min;
                            minall_num_edges = min_num_edges;
                            is_minall_inf = false;
                        }
                    }

                    // init touched data
                    init_used_dijkstra();
                }

                // now return path found
                if ( is_minall_inf == true ) 
                    leda::error_handler(999,"MIN_CYCLE_BASIS: no cycle found :(");

                return minall;
            }



            // data structures
            int n,m;
            PT p;
            const graph& G;
            const edge_array< T > &len; // edge lengths
            const edge_num& enumb;      // edge numbering
            p_queue<T,pnode> PQ;        // priority queue

            // sortseq keys on node id of a node in levelg
            // SSitem
            // contains first()  = reverse index on priority queue, 
            //                     node(in levelg) -> pq_item
            //          second() = distance of shortest path in 
            //                     levelg to this node
            //          third()  = number of edges of shortest path in 
            //                     levelg to this node
            //          fourth() = predecessor as three_tuple in levelg 
            //                     to this node
            //                     preditem
            //                     contains first()  = edge e of g
            //                              second() = node id of other 
            //                                         end in levelg
            //                              third()  = node in g on other end
            sortseq< PT, SSitem > SS;

            array< etype > cycle;                // incident vector of cycles, indexed on 
            // numbering of edges (enumb(e))
            node_array< PT > nodeid;             // nodes' ids   of g
            node_array< unsigned short > marked; // marked nodes of g 
            node_list marked_touched;            // touched marked nodes 
            edge_array< PT > Xe;                 // help set to construct the cycle
        };

    // now undefine, all defined stuff for SSitem and preditem
#undef mcb_revi
#undef mcb_dist
#undef mcb_num_edges
#undef mcb_pred
#undef mcb_edge
#undef mcb_other_node_index
#undef mcb_other_node

    
} // end of mcb namespace

#endif // DSIGNED_H

/* ex: set ts=4 sw=4 sts=4 et: */
