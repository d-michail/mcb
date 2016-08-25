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

/*! \file util.h
 *  \brief Various helper functions
 */

#ifndef UTIL_H
#define UTIL_H

#include <LEP/mcb/spvecgf2.h>
#include <LEP/mcb/spvecfp.h>
#include <LEP/mcb/edge_num.h>

#ifdef LEDA_GE_V5
#include <LEDA/graph/graph.h>
#include <LEDA/graph/edge_set.h>
#include <LEDA/graph/edge_array.h>
#else
#include <LEDA/graph.h>
#include <LEDA/edge_set.h>
#include <LEDA/edge_array.h>
#endif

namespace mcb 
{ 

#if defined(LEDA_NAMESPACE)
    using leda::graph;
    using leda::array;
    using leda::node;
    using leda::edge;
    using leda::edge_array;
    using leda::edge_set;
    using leda::node_array;
    using leda::d_int_set;
#endif
    
    //@{

    //
    // A functor which orients an undirected cycle.
    // 
    template<class Container = mcb::spvecgf2>
    class CycleOrienter
    {
        public:
            CycleOrienter( const graph& g_, 
                         const edge_num& enumb_ )
                : g(g_), enumb(enumb_), adj(g, edge_set(g))
            {
            }

            mcb::spvecfp operator()( const Container& cycle )
            {
                CreateAdjacencyList( cycle );
                edge_array<etype> dircycle(g, 0);
                TraverseCycle( dircycle );
                return MakeSparseRepresentation( dircycle );
            }

        private:
        
            // assumes input is initialized to constructor(g,0)
            void TraverseCycle( edge_array<etype>& dircycle )
            {
                // preconditions
                assert( s != nil );
                if ( ! CheckCycle() )
                    leda::error_handler(999,"CycleOrienter: not a cycle!");
                    
                edge e;
                node v = s, u;
                while( adj[v].size() > 0 )
                {
                    e = adj[v].choose();
                    if ( v == g.source(e) )
                        dircycle[ e ] = 1;
                    else
                        dircycle[ e ] = -1;

                    u = g.opposite( v, e );
                    adj[v].del( e );
                    adj[u].del( e );
                    v = u;
                }
            }

            mcb::spvecfp MakeSparseRepresentation( edge_array<etype>& dircycle )
            {
                mcb::spvecfp ret;
                edge e;
                for( int i = 0; i < g.number_of_edges(); ++i )
                {
                    e = enumb(i);
                    if ( dircycle[e] > 0 )
                        ret.append( i, 1 );
                    else if ( dircycle[e] < 0 )
                        ret.append( i, -1 );
                }
                return ret;
            }

            void InitializeAdjacencyList() 
            {
                node v;
                forall_nodes(v,g)
                    adj[v].clear();
            }

            void CreateAdjacencyList( const mcb::spvecgf2& cycle ) 
            {
                InitializeAdjacencyList();
    
                int i;
                edge e = nil;
                forall( i, cycle ) { 
                    e = enumb(i);
                    adj[ g.source(e) ].insert(e);
                    adj[ g.target(e) ].insert(e);
                }
                s = g.source(e);
            }

            void CreateAdjacencyList( const leda::d_int_set& cycle ) 
            {
                InitializeAdjacencyList();
    
                edge e = nil;
                forall_edges(e,g) { 
                    if ( ! cycle.member( enumb(e) ) )
                        continue;
                    adj[ g.source(e) ].insert(e);
                    adj[ g.target(e) ].insert(e);
                    s = g.source(e);
                }
            }


            bool CheckCycle()
            {
                node v;
                forall_nodes( v, g )
                    if( adj[v].size() %2 == 1 )
                        return false;
                return true;   
            }

            const graph& g;
            const mcb::edge_num& enumb;

            node_array< edge_set > adj;
            node s;
    };

    //
    // A functor which computes the weight of a cycle
    // 
    template<class W,class Container = mcb::spvecfp>
    class ComputeWeight
    {
        public:
            ComputeWeight( const graph& g_, 
                           const edge_array<W>& len_,
                           const edge_num& enumb_ )
                : g(g_), enumb(enumb_), len(len_)
            {
            }

            W operator()( const Container& cycle )
            {
                return ComputeCycleWeight( cycle );
            }

        private:

            W ComputeCycleWeight( const mcb::spvecfp& cycle )
            {   
                W ret = W();
                leda::list_item it = cycle.first();
                while(it!=nil)
                {
                    int index = cycle.index(it);
                    ret += len[ enumb( index ) ];
                    it = cycle.succ(it);
                }
                return ret;
            }

            W ComputeCycleWeight( const mcb::spvecgf2& cycle )
            {   
                W ret = W();
                int j = 0;
                forall( j, cycle )
                    ret += len[ enumb( j ) ];
                return ret;
            }

            W ComputeCycleWeight( const leda::d_int_set& cycle )
            {   
                W ret = W();
                edge e = nil;
                forall_edges(e,g)
                    if ( cycle.member( enumb(e) ) )
                        ret += len[e];
                return ret;
            }
        
            const graph& g;
            const mcb::edge_num& enumb;
            const edge_array<W>& len;
    };


    //@}

} // namespace mcb end

#endif  // UTIL_H

/* ex: set ts=4 sw=4 sts=4 et: */


