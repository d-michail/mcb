
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

/*! \file spanner.h
 *  \brief Algorithms to compute a sparse spanner of an undirected graph.
 *
 */

#ifndef MCB_SPANNER_H
#define MCB_SPANNER_H

#include <LEP/mcb/config.h>

#ifdef LEDA_GE_V5
#include <LEDA/graph/graph.h>
#include <LEDA/graph/edge_array.h>
#include <LEDA/graph/edge_map.h>
#include <LEDA/graph/node_array.h>
#include <LEDA/graph/node_map.h>
#include <LEDA/core/array.h>
#include <LEDA/core/list.h>
#else
#include <LEDA/graph.h>
#include <LEDA/edge_array.h>
#include <LEDA/edge_map.h>
#include <LEDA/node_array.h>
#include <LEDA/node_map.h>
#include <LEDA/array.h>
#include <LEDA/list.h>
#endif

#include <LEP/mcb/edge_num.h>
#include <LEP/mcb/spvecgf2.h>
#include <LEP/mcb/ushortpath.h>

#include <vector>
#include <algorithm>
#include <iterator>

// start our namespace
namespace mcb 
{ 

#if defined(LEDA_NAMESPACE)
    using leda::graph;
    using leda::array;
    using leda::edge;
    using leda::edge_array;
    using leda::edge_map;
    using leda::node;
    using leda::node_array;
    using leda::node_map;
    using leda::error_handler;
    using leda::list;
#endif

    namespace detail 
    {

        // define a class to sort the edges of a graph
        template<class W>
            class edge_sorter
            {
                public:
                    edge_sorter( const graph& g, 
                            const edge_array<W>& length,
                            const mcb::edge_num& edgenumb ) : G(g), len(length), enumb(edgenumb) {}
                    ~edge_sorter() {}

                    // a sort function
                    void sort(array<edge>& sorted) 
                    { 

                        std::vector< wedge > sorted_wedges( G.number_of_edges() );

                        // copy edges
                        edge e;
                        int i = 0;
                        forall_edges( e, G ) { 
                            sorted_wedges[ i++ ] = wedge( enumb( e ), len[e] );
                        }

                        // sort using STL
                        std::sort( sorted_wedges.begin(), sorted_wedges.end() );

                        // copy result
                        sorted.resize( G.number_of_edges() );
                        i = 0;
                        typename std::vector<wedge>::iterator it = sorted_wedges.begin();
                        while( it != sorted_wedges.end() ) 
                        {
                            sorted[ i++ ] = enumb( (*it).get_edge_num() );
                            it++;
                        }
                    }

                private:
                    // graph and edge lengths
                    const graph& G;
                    const edge_array<W>& len;
                    const mcb::edge_num& enumb;

                    // a weighted edge for sorting
                    class wedge {

                        public: 
                            wedge() { en = 0; weight = W(); }
                            wedge( int i, const W& w ) : en(i), weight(w) {}
                            ~wedge() {}

                            bool operator<( const wedge& ew ) const { return weight < ew.weight; }

                            int get_edge_num() { return en; }
                            W   get_edge_weight() { return weight; }

                        private:
                            int en;    // number of edge
                            W weight;  // weight of edge
                    };

            };


        /*! \brief Compute a sparse \f$(2k-1)\f$-spanner of a weighted graph.
         *  Even if the graph is directed the spanner will be a \f$(2k-1)\f$-spanner
         *  of the underlying undirected graph.
         * 
         *  \param g The original graph
         *  \param len The edge lengths
         *  \param k Returns a \f$2k-1\f$ spanner.
         *  \param spanner The returned spanner graph.
         *  \param node_g_to_spanner Node mapping from graph to spanner.
         *  \param node_spanner_to_g Node mapping from spanner to graph.
         *  \param edge_g_to_spanner Edge mapping from graph to spanner.
         *  \param edge_spanner_to_g Edge mapping from spanner to graph.
         *  \param enumb  Edge numbering of the original graph.
         *
         *  \pre The edge numbering should be initialized on the original graph.
         *  \pre \f$k\f$ should be \f$\ge 1\f$.
         *  \pre Edge lengths should be non-negative.
         */

        template<class W>
            void SPANNER( const graph& g,
                    const edge_array<W>& len,
                    const int k,
                    graph& spanner,
                    node_map<node>& node_g_to_spanner,
                    node_map<node>& node_spanner_to_g,
                    edge_map<edge>& edge_g_to_spanner,
                    edge_map<edge>& edge_spanner_to_g,
                    const mcb::edge_num& enumb
                    ) 
            {
                // check preconditions
#if ! defined(LEDA_CHECKING_OFF)
                if ( k < 1 ) 
                    error_handler(999,"SPANNER: k must be >= 1");
#endif

                // sort edges of graph
                array<edge> sorted_edges;
                edge_sorter<W> S( g, len, enumb );
                S.sort( sorted_edges );

                // construct node set of spanner
                spanner.clear();
                edge_spanner_to_g.init( spanner, nil );
                edge_g_to_spanner.init( g, nil );
                node_spanner_to_g.init( spanner, nil );
                node_g_to_spanner.init( g, nil );
                node v, u;
                forall_nodes( v, g ) {
                    u = spanner.new_node();
                    node_spanner_to_g[ u ] = v;
                    node_g_to_spanner[ v ] = u;
                }

                // structures for BFS on spanner
                detail::ubfs csp( spanner );

                // construct edge set of spanner
                int i = 0;
                edge e;
                node spanner_v, spanner_u;
                for( i = 0; i < g.number_of_edges(); ++i ) { 

                    e = sorted_edges[ i ];
                    v = g.source(e);
                    u = g.target(e);
                    spanner_v = node_g_to_spanner[ v ];
                    spanner_u = node_g_to_spanner[ u ];

#if ! defined(LEDA_CHECKING_OFF)
                    if ( spanner_v == spanner_u ) 
                        error_handler(999,"UMCB_SPANNER: self loops?");
#endif

                    // check if there is a path of length 2k-1 between
                    // the endpoints of the edge
                    csp.compute_shortest_path( spanner_v, spanner_u, 2*k-1 );

                    if ( csp.is_reachable( spanner_u ) ) 
                    { 
                        edge_g_to_spanner[ e ] = nil;
                    }
                    else 
                    { 
                        // add edge to the spanner
                        edge_g_to_spanner[ e ] = spanner.new_edge( spanner_v, spanner_u );
                        edge_spanner_to_g[ edge_g_to_spanner[ e ] ] = e;
                    }

                }
            }

    }

}

#endif  // MCB_SPANNER_H

/* ex: set ts=4 sw=4 sts=4 et: */
