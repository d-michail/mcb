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
// $Id: edge_num.C 7008 2008-04-15 12:07:38Z dmichail $ 
//

/*! \file edge_num.C
 *  \brief Implementation of the edge numbering.
 */

#include <LEP/mcb/edge_num.h>
#include <LEDA/core/queue.h>

namespace mcb { 

#if defined(LEDA_NAMESPACE)
    using leda::graph;
    using leda::edge;
    using leda::edge_array;
    using leda::node;
    using leda::node_set;
    using leda::queue;
    using leda::error_handler;
#endif

    edge_num::edge_num(): 
        n(0), m(0), k(0)
    {
    }

    edge_num::edge_num( const graph& G ): 
        n(G.number_of_nodes()), 
        m(G.number_of_edges()), 
        index( G.number_of_edges() ),
        rindex(G) 
    { 
        create_numbering( G );
    }

    edge_num::edge_num( const edge_num& enumb ) { 
        n = enumb.n;
        m = enumb.m;
        k = enumb.k;
        index.resize( m );
        for( int i = 0 ; i < m ; ++i ) 
            index[ i ] = enumb.index[ i ];
        rindex = enumb.rindex;
    }

    edge_num::~edge_num( void ) {
    }

    edge_num& edge_num::operator=( const edge_num& x )
    {
        if ( this == &x )
            return *this;
        n = x.n;
        m = x.m;
        k = x.k;
        index = x.index;
        rindex = x.rindex;
        return *this;
    }

    void edge_num::create_numbering( const graph& G ) { 
        edge_array<bool> tree( G );
        k = construct_tree( G, tree );
        int N = m - n + k;

        edge e;
        int y = 0 , l = N ;
        forall_edges( e , G ) { 
            if ( tree[ e ] == false ) { 
                index[y] = e; 
                rindex[ e ] = y; 
                y++; 
            }
            else { 
                index[l] = e; 
                rindex[ e ] = l; 
                l++; 
            }
        }
    }

    int edge_num::construct_tree( const graph& g, edge_array<bool>& tree ) { 
        // initialize
        node v = nil, u = nil, w = nil;
        if ( g.number_of_nodes() == 0 ) 
            return 0;
        queue<node> Q;
        node_set unreached( g );
        tree.init( g, false );
        forall_nodes(v,g) 
            unreached.insert( v );
        edge e;
        int c = 0;

        // run
        while( ! unreached.empty() ) { 

            v = unreached.choose();
            unreached.del( v );
            Q.append( v );

            while( ! Q.empty() ) { 
                u = Q.pop();

                // Unfortunately after LEDA 6.0 forall_inout_edges does
                // not iterate over edges in undirected graphs and thus
                // we need this very ugly code duplication in order to 
                // treat directed graphs as undirected.
                if ( g.is_undirected() ) 
                    forall_adj_edges( e, u ) {
                        w = g.opposite( u,e );
                        if( unreached.member(w) == false ) 
                            continue;
                        tree[e] = true;
                        Q.append( w );
                        unreached.del( w ); 
                    }
                else 
                    forall_inout_edges( e, u ) {
                        w = g.opposite( u,e );
                        if( unreached.member(w) == false ) 
                            continue;
                        tree[e] = true;
                        Q.append( w );
                        unreached.del( w ); 

                    }
            }
            c++;
        }
        return c;
    }


} // end namespace mcb

/* ex: set ts=4 sw=4 sts=4 et: */


