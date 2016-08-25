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

/*! \file superset.h
 *  \brief Builder constructs for Horton's Superset
 *
 *  \sa mcb::edge_num mcb::spvecgf2
 */

#ifndef SUPERSET_H
#define SUPERSET_H

#include <LEP/mcb/config.h>

#ifdef LEDA_GE_V5
#include <LEDA/graph/graph.h>
#include <LEDA/graph/edge_array.h>
#include <LEDA/graph/node_array.h>
#include <LEDA/graph/node_set.h>
#include <LEDA/core/d_int_set.h>
#include <LEDA/core/array.h>
#include <LEDA/core/list.h>
#include <LEDA/core/sortseq.h>
#include <LEDA/core/tuple.h>
#include <LEDA/system/error.h>
#else
#include <LEDA/graph.h>
#include <LEDA/d_int_set.h>
#include <LEDA/edge_array.h>
#include <LEDA/node_array.h>
#include <LEDA/node_set.h>
#include <LEDA/array.h>
#include <LEDA/list.h>
#include <LEDA/sortseq.h>
#include <LEDA/tuple.h>
#include <LEDA/error.h>
#endif

#include <LEP/mcb/edge_num.h>
#include <LEP/mcb/udijkstra.h>

// start our namespace
namespace mcb 
{ 

#if defined(LEDA_NAMESPACE)
    using leda::graph;
    using leda::array;
    using leda::edge;
    using leda::edge_array;
    using leda::d_int_set;
    using leda::list;
    using leda::sortseq;
    using leda::three_tuple;
    using leda::two_tuple;
    using leda::node_set;
#endif

    template<class W>
        class HortonSupersetBuilder
        {
            public:
                HortonSupersetBuilder( const graph& G_,
                        const edge_array<W>& len_, 
                        const mcb::edge_num& enumb_ )
                    : G(G_), len(len_), enumb(enumb_), pred(G_)
                { 
                }

                void construct( sortseq< three_tuple<W,int,int>, d_int_set >& super )
                {
                    // initialize
                    node v,w,u,vz,ezs,ezt; 
                    edge e,ez;

                    int c = 0;
                    node_array<int> degree(G,0);
                    node_set touched( G );
                    bool dege;

                    W min;

                    ConstructPaths();

                    forall_nodes( v , G ) { 
                        forall_edges( e, G ) { 

                            dege = false;

                            w = G.source(e);
                            u = G.target(e);

                            // valid?
                            if ( v == w || v == u || 
                                    pred[ v ][ w ] == nil || pred[ v ][ u ] == nil ||
                                    pred[ v ][ w ] == e || pred[ v ][ u ] == e || 
                                    pred[ w ][ v ] == e || pred[ u ][ v ] == e ) 
                                continue;

                            // try to construct cycle
                            C.clear();

                            // insert e
                            C.insert( enumb(e) );
                            ezs = G.source( e );
                            ezt = G.target( e );
                            degree[ ezs ]++;
                            degree[ ezt ]++;
                            touched.insert( ezs );
                            touched.insert( ezt );

                            min = len[ e ];

                            // create two paths
                            int en;
                            node x,y; 

                            // make sure path is unique
                            if ( v->id() < w->id() ) { y = v; x = w; }
                            else { y = w; x = v; }

                            while( pred[ y ][ x ] != nil && dege == false ) {
                                ez = pred[ y ][ x ];
                                en = enumb( ez );
                                if ( ! C.member( en ) ) {
                                    C.insert( en );
                                    ezs = G.source( ez );
                                    ezt = G.target( ez );
                                    degree[ ezs ]++;
                                    degree[ ezt ]++;
                                    if ( degree[ ezs ] > 2 || degree[ ezt ] > 2 )
                                        dege = true;
                                    touched.insert( ezs );
                                    touched.insert( ezt );
                                    min += len[ ez ];
                                }
                                x = G.opposite( ez, x );
                            }

                            if ( v->id() < u->id() ) { y = v; x = u; }
                            else { y = u; x = v; }

                            while( pred[ y ][ x ] != nil && dege == false ) {
                                ez = pred[ y ][ x ];
                                en = enumb( ez );
                                if ( ! C.member( en ) ) {
                                    C.insert( en );
                                    ezs = G.source( ez );
                                    ezt = G.target( ez );
                                    degree[ ezs ]++;
                                    degree[ ezt ]++;
                                    if ( degree[ ezs ] > 2 || degree[ ezt ] > 2 )
                                        dege = true;
                                    touched.insert( ezs );
                                    touched.insert( ezt );
                                    min += len[ ez ];
                                }
                                x = G.opposite( ez, x );
                            }


                            // init degree array, and check for degeneracy
                            while( touched.empty() == false ) {
                                vz = touched.choose();
                                touched.del( vz );
                                if ( degree[ vz ] == 1 ) dege = true;
                                degree[ vz ] = 0;
                            }

                            if ( dege == true ) continue;

                            // add cycle to superset
                            three_tuple<W,int,int> tt( min, C.size(), c++ ); 
                            super.insert( tt, C );
                        }
                    }
                }

            private:

                void ConstructPaths() { 
                    node v;
                    node_array<W> dist(G);

                    forall_nodes( v, G ) {
                        pred[v].init( G, nil );
                        mcb::undirected_dijkstra( G, v, len, dist, pred[v] );
                    }
                }

                HortonSupersetBuilder( const HortonSupersetBuilder& );
                HortonSupersetBuilder& operator=( const HortonSupersetBuilder& );

            private:
                const graph& G;
                const edge_array<W>& len;
                const mcb::edge_num& enumb;
                node_array< node_array<edge> > pred;
                d_int_set C;
        };


    template<class W>
    class HortonSuperset
    {
        public:
            HortonSuperset( const graph& G_,
                    const edge_array<W>& len_, 
                    const mcb::edge_num& enumb_ )
                : super( leda::compare )
            { 
                HortonSupersetBuilder<W> hs( G_, len_, enumb_ );
                hs.construct( super );
            }

            W get_shortest_odd_cycle( const d_int_set& S, d_int_set& C, bool eraseFound = true ) { 
                leda::seq_item s = super.min_item();
                bool f = false;
                W ret = W();
                while ( s != nil ) {
                    if ( ( (super[s]).intersect( S ) ).size() % 2 == 1 ) { // is odd
                        C = super[s];
                        ret = super.key( s ).first();
                        if ( eraseFound ) 
                            super.del_item( s );
                        f = true; // found
                        break;
                    }
                    s = super.succ( s );
                }
                if ( f == false )
                    leda::error_handler(999,"MIN_CYCLE_BASIS (HYBRID): superset contains no MCB :(");
                return ret;
            }   

        private:
            HortonSuperset( const HortonSuperset& );
            HortonSuperset& operator=( const HortonSuperset& );

            sortseq< three_tuple<W,int,int>, d_int_set > super;
    };

} // namespace mcb end

#endif  // SUPERSET_H

/* ex: set ts=4 sw=4 sts=4 et: */


