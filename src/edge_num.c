//---------------------------------------------------------------------
// File automatically generated using notangle from DMIN_CYCLE_BASIS.lw
//
// mails and bugs: Dimitrios Michail <dimitrios.michail@gmail.com>
//---------------------------------------------------------------------
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
// Copyright (C) 2004-2005 - Dimitrios Michail

#include <LEP/mcb/edge_num.h>

namespace mcb
{

#if defined(LEDA_NAMESPACE)
    using leda::graph;
    using leda::edge;
    using leda::edge_array;
    using leda::node;
    using leda::node_set;
    using leda::b_queue;
    using leda::error_handler;
#endif



      edge_num::edge_num( const graph & G ):m( G.number_of_edges(  ) ),
	n( G.number_of_nodes(  ) ), rindex( G )
    {
	index = new edge[m];
	create_numbering( G );
    }

    edge_num::edge_num( const edge_num & enumb )
    {
	m = enumb.m;
	n = enumb.n;
	k = enumb.k;
	index = new edge[m];
	for ( int i = 0; i < m; i++ )
	    index[i] = enumb.index[i];
	rindex = enumb.rindex;
    }

    edge_num::~edge_num( void )
    {
	if ( index != nil )
	    delete[]index;
    }


    void edge_num::create_numbering( const graph & G )
    {
	edge_array < bool > tree( G );
	k = construct_tree( G, tree );
	int N = m - n + k;

	edge e;
	int y = 0, l = N;

	forall_edges( e, G ) {
	    if ( tree[e] == false ) {
		index[y] = e;
		rindex[e] = y;
		y++;
	    } else {
		index[l] = e;
		rindex[e] = l;
		l++;
	    }
	}
    }

    int edge_num::construct_tree( const graph & g, edge_array < bool > &tree )
    {
	// initialize
	node v, u, w;

	if ( g.number_of_nodes(  ) == 0 )
	    return 0;
	b_queue < node > Q( g.number_of_nodes(  ) );
	node_set unreached( g );

	tree.init( g, false );
	forall_nodes( v, g ) unreached.insert( v );
	edge e;
	int c = 0;

	// run
	while ( unreached.empty(  ) == false ) {

	    v = unreached.choose(  );
	    unreached.del( v );
	    Q.append( v );

	    while ( Q.empty(  ) == false ) {
		u = Q.pop(  );

		forall_inout_edges( e, u ) {
		    w = g.opposite( u, e );
		    if ( unreached.member( w ) == false )
			continue;

		    unreached.del( w );
		    tree[e] = true;
		    Q.append( w );
		}
	    }
	    c++;

	}
	return c;
    }


}				// end namespace mcb

/* ex: set ts=8 sw=4 sts=4 noet: */
