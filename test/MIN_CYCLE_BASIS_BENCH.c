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


#line 4551 "MIN_CYCLE_BASIS.lw"
#include <iostream>
#include <LEP/mcb/min_cycle_basis.h>

#ifdef LEDA_GE_V5
#include <LEDA/system/basic.h>
#include <LEDA/graph/graph.h>
#include <LEDA/graph/graph_misc.h>
#include <LEDA/core/random.h>
#else
#include <LEDA/basic.h>
#include <LEDA/graph.h>
#include <LEDA/graph_misc.h>
#include <LEDA/random.h>
#endif

#if defined(LEDA_NAMESPACE)
using namespace leda;
#endif


#line 4672 "MIN_CYCLE_BASIS.lw"
// execute and measure CPU time taken
float execute( graph& G, edge_array<int>& len) { 
    float T;
    used_time( T );
    mcb::edge_num enumb( G );
    array< d_int_set > mcb;
    array< d_int_set > proof;
    mcb::MIN_CYCLE_BASIS_DEPINA( G, len, mcb, proof, enumb);
    return used_time( T );
}

// execute and measure CPU time taken
float execute( graph& G ) { 
    float T;
    used_time( T );
    mcb::edge_num enumb( G );
    array< d_int_set > mcb;
    array< d_int_set > proof;
    mcb::MIN_CYCLE_BASIS_DEPINA( G, mcb, proof, enumb);
    return used_time( T );
}

// execute and measure CPU time taken
float execute_sparse( graph& G ) { 
    float T;
    used_time( T );
    mcb::edge_num enumb( G );
    array< mcb::spvecgf2 > mcb;
    array< mcb::spvecgf2 > proof;
    mcb::MIN_CYCLE_BASIS_DEPINA( G, mcb, proof, enumb );
    return used_time( T );
}

// execute and measure CPU time taken
float execute_sparse( graph& G, edge_array<int>& len ) { 
    float T;
    used_time( T );
    mcb::edge_num enumb( G );
    array< mcb::spvecgf2 > mcb;
    array< mcb::spvecgf2 > proof;
    mcb::MIN_CYCLE_BASIS_DEPINA( G, len, mcb, proof, enumb );
    return used_time( T );
}

// execute and measure CPU time taken
float executeh( graph& G, edge_array<int>& len) { 
    float T;
    used_time( T );
    mcb::edge_num enumb( G );
    array< d_int_set > mcb;
    array< d_int_set > proof;
    mcb::MIN_CYCLE_BASIS_HYBRID( G, len, mcb, proof, enumb);
    return used_time( T );
}

// execute and measure CPU time taken
float executeh( graph& G ) { 
    float T;
    used_time( T );
    mcb::edge_num enumb( G );
    array< d_int_set > mcb;
    array< d_int_set > proof;
    mcb::MIN_CYCLE_BASIS_HYBRID( G, mcb, proof, enumb);
    return used_time( T );
}


#line 4644 "MIN_CYCLE_BASIS.lw"
// create random graph
void create_graph( int n, 
		   int m,
		   graph& G, 
		   edge_array<int>& len ) {

	random_simple_undirected_graph( G, n, m );

	// meet preconditions
	Delete_Loops( G );
	Make_Simple( G );
	G.make_undirected();

	// give non-negative edge lengths
	len.init( G );
	edge e;
	forall_edges (e,G)
		len[e] = rand_int( 0, 65536 );
}



#line 4573 "MIN_CYCLE_BASIS.lw"
int main() { 
	int n; 
	double p;

	// simulate means than we solve weighted with an edge array
	// where len[e] = 1 forall edges e of G
	std::cout << "#n - p - depina - hybrid - depina unweighted";
	std::cout << " - hybrid unweighted - depina simulate unweighted";
	std::cout << " - hybrid simulate unweighted "; 
	std::cout << " - depinaSP  -  depinaSP unweighted" << std::endl;
	int times = 5; 
	
	while( std::cin ) { 
		
		std::cin >> n >> p;
		if ( n <= 0 || p < 0 || p > 1  ) break;
	
		float t = 0.0, tu = 0.0, th = 0.0;
		float thu = 0.0, tus = 0.0, thus = 0.0;
		float tspu = 0.0, tsp = 0.0;

		// average 
		for( int i = 0; i < times; i++ ) 
		{ 
			// create random graph
			graph G;
			edge_array<int> len;
			create_graph( n, int( p * n * (n-1) / 2 ) , G, len );
			edge_array<int> lenu( G, 1 );
			t += execute( G, len );
			tu += execute( G );
			th += executeh( G, len );
			thu += executeh( G );
			tus += execute( G, lenu );
			thus += executeh( G, lenu );
			tspu += execute_sparse( G );
			tsp += execute_sparse( G, len );
			
		}
		t /= times;
		tu /= times;
		th /= times;
		thu /= times;
		tus /= times;
		thus /= times;
		tsp /= times;
		tspu /= times;
		
		std::cout << n << "\t" << p;
		std::cout << "\t\t" << t ; 
		std::cout << "\t\t" << th;
		std::cout << "\t\t" << tu;
		std::cout << "\t\t" << thu;
		std::cout << "\t\t" << tus;
		std::cout << "\t\t" << thus; 
		std::cout << "\t\t" << tsp; 
		std::cout << "\t\t" << tspu << std::endl;
	}
	return 0;
}

#line 6217 "MIN_CYCLE_BASIS.lw"
/* ex: set ts=8 sw=4 sts=4 noet: */


