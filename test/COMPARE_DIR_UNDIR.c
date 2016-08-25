#line 6220 "MIN_CYCLE_BASIS.lw"
//---------------------------------------------------------------------
// File automatically generated using notangle from DMIN_CYCLE_BASIS.lw
//
// emails and bugs: Dimitrios Michail <dimitrios.michail@gmail.com>
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


#line 630 "MIN_CYCLE_BASIS.lw"
// program to execute both directed and undirected case
// on random graphs and compare result

#include <iostream>
#include <LEP/mcb/mcb.h>

#ifdef LEDA_GE_V5
#include <LEDA/core/random_source.h>
#include <LEDA/graph/graph.h>
#include <LEDA/graph/graph_misc.h>
#include <LEDA/system/assert.h>
#else
#include <LEDA/random_source.h>
#include <LEDA/graph.h>
#include <LEDA/graph_misc.h>
#include <LEDA/assert.h>
#endif

#if defined(LEDA_NAMESPACE)
using namespace leda;
#endif

edge_array<int> len;


#line 8265 "MIN_CYCLE_BASIS.lw"
// write edge weight to ostream
void write_edge_weight( std::ostream& o, const graph* G, const edge e ) { 
    o << "    label \"" << len[e] << "\"" << std::endl;
}


#line 715 "MIN_CYCLE_BASIS.lw"
// create graph
void create_graph( int n, int m, graph& G, edge_array<int>& len ) { 

    // create random graph
    random_graph( G, n, m , true, true, true );

    // give non-negative edge lengths
    len.init( G );
    edge e;
    forall_edges (e,G)
	len[e] = rand_int( 1, 65536 );
}

#line 6217 "MIN_CYCLE_BASIS.lw"
/* ex: set ts=8 sw=4 sts=4 noet: */




#line 657 "MIN_CYCLE_BASIS.lw"
int main() { 
    int i = 0, n, m;

    while( true ) { 
	std::cout << "example " << i++ << std::endl;

	// create random graph
	graph G;

	n = rand_int( 1, 40 );
	m = rand_int( 1, n*(n-1)/2 );
	create_graph( n, m, G, len );

	// write graph to file
	G.write_gml("graph.input", 0, write_edge_weight );

	mcb::edge_num enumb( G );
	int N = enumb.dim_cycle_space();
	int c = enumb.num_weak_connected_comp();
	m = G.number_of_edges();
	n = G.number_of_nodes();

	std::cout << " N = m - n + c = " << G.number_of_edges(); 
	std::cout << " - " << G.number_of_nodes() << " + ";
	std::cout << c << " = " << N << std::endl;

	if ( N <= 0 ) continue;

	float Tdirstart, Tdirend;
	float Tundirstart, Tundirend;

	leda::used_time( Tdirstart );
	array< list<edge> > mcbdir;
	int wdir = mcb::MCB( G, len, mcbdir );
	Tdirend = used_time( Tdirstart );

	// make graph undirected
	G.make_undirected();

	leda::used_time( Tundirstart );
	array< list<edge> > mcbundir;
	int wundir = mcb::MCB( G, len, mcbundir );
	Tundirend = used_time( Tundirstart );

	if ( N > 0 ) { 
	    std::cout << "directed MCB weight: " << wdir ; 
	    std::cout << " in time: " << Tdirend << " sec" << std::endl;
	    std::cout << "undirected MCB weight: " << wundir;
	    std::cout << " in time: " << Tundirend << " sec" << std::endl;
	}

	std::cout << std::endl;
    }
    return 0;
}

