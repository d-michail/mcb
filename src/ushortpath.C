
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

/*! \file ushortpath.C
 *  \brief Undirected shortest paths
 */

#include <LEP/mcb/ushortpath.h>

namespace mcb { 

#if defined(LEDA_NAMESPACE)
    using leda::graph;
    using leda::edge;
    using leda::edge_array;
    using leda::node;
    using leda::node_array;
    using leda::list;
    using leda::list_item;
    using leda::error_handler;
#endif

    namespace detail
    {

        ubfs::ubfs( const graph& G ) 
            : g(G), pred_(G, nil), dist_( G, 0 ), last_s( nil )
        {
        }

        ubfs::~ubfs() {}

        // assumption: entries in predecessor which are not nil have to 
        //             belong into Q
        void ubfs::compute_shortest_path( const node& s, const node& t, int limit )
        {
            // empty Q assuming Q was the visited nodes in a previous run
            while( Q.empty() == false ) 
                pred_[ Q.pop() ] = nil;

            // start BFS
            Q.push( s );
            dist_[ s ] = 0; 
            pred_[ s ] = nil;
            last_s = s;
            for( list_item it = Q.first(); it; it = Q.succ(it) )
            {
                node v = Q[it];

                edge e;
                if ( g.is_undirected() )
                    forall_adj_edges(e,v)
                    { 
                        node w = g.opposite(v,e);
                        if ( pred_[w] == nil && w != s && dist_[v] < limit ) { 
                            Q.append(w);
                            dist_[w] = dist_[v] + 1;
                            pred_[w] = e;

                            if ( w == t ) 
                                return;
                        }
                    }
                else // duplicate code from above due to change of LEDA-6.0
                    forall_inout_edges(e,v)
                    { 
                        node w = g.opposite(v,e);
                        if ( pred_[w] == nil && w != s && dist_[v] < limit ) { 
                            Q.append(w);
                            dist_[w] = dist_[v] + 1;
                            pred_[w] = e;

                            if ( w == t ) 
                                return;
                        }
                    }
            }
            return;
        }

    } // end namespace detail

} // end namespace mcb

/* ex: set ts=4 sw=4 sts=4 et: */
