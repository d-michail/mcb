//---------------------------------------------------------------------
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
// Copyright (C) 2004-2006 - Dimitrios Michail


#include <iostream>
#include <LEP/mcb/mcb_approx.h>

#ifdef LEDA_GE_V5
#include <LEDA/graph/graph_misc.h>
#include <LEDA/graph/gml_graph.h>
#include <LEDA/core/map.h>
#else
#include <LEDA/graph_misc.h>
#include <LEDA/gml_graph.h>
#include <LEDA/map.h>
#endif

#if defined(LEDA_NAMESPACE)
using namespace leda;
#endif

// read an unweighted graph in GML format and 
// find a minimum cycle basis. 
// Output the time taken
int main( int argc, char *argv[] ) { 

	int k;
	if ( argc > 1 )  
	    k = atoi( argv[1] );
	else 
	    k = 2;

        // initialize graph
        graph G;
        
        // create parser and read from standard input
        gml_graph parser( G );
        if ( parser.parse( std::cin ) == false ) return -1;
        
        // execute
        float T,T1;
        
        leda::used_time( T ); // start time
 
	// create uniform lengths
	leda::edge_array<int> len( G, 1 );

	// numbering and structures to store approx MCB
        mcb::edge_num enumb( G );
        array< mcb::spvecgf2 > mcb;

        mcb::UMCB_APPROX<int>( G, len, k, mcb, enumb );

        T1 = leda::used_time( T ); // finish time

#ifndef NO_OUTPUT_HISTOGRAM
	map< int,int > sizes;
	int s;
	for( int i = 0; i < enumb.dim_cycle_space(); ++i ) 
	{
	    s = mcb[i].size();
	    if ( sizes.defined( s ) )
		sizes[ s ] = sizes[ s ] + 1;
	    else
		sizes[ s ] = 1;
	}

	for( int i = 0; i <= G.number_of_edges(); i++ ) 
	{
	    if ( sizes.defined( i ) ) 
		std::cout << "length " << i << " - number of cycles " << sizes[i] << std::endl;
	}
#endif
        
        std::cout << "time taken: " << T1 << std::endl;

        return 0;
}

/* ex: set ts=8 sw=4 sts=4 noet: */


