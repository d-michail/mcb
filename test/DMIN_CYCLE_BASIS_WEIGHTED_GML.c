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


#include <iostream>
#include <stdio.h>
#include <LEP/mcb/dir_min_cycle_basis.h>

#ifdef LEDA_GE_V5
#include <LEDA/graph/graph_misc.h>
#include <LEDA/graph/gml_graph.h>
#include <LEDA/graph/edge_map.h>
#else
#include <LEDA/graph_misc.h>
#include <LEDA/gml_graph.h>
#include <LEDA/edge_map.h>
#endif

#if defined(LEDA_NAMESPACE)
using namespace leda;
#endif

// length
edge_map<int> lenmap;

// read edge weight
// in the following form: label "weight" or label "uniquenumber (weight)"
bool get_edge_weight( const gml_object* gobj, graph* G, edge e ) {
    char* str = gobj->get_string();
    int w, tw;
    if ( sscanf( str, "%d (%d)", &tw, &w ) == 2 ) {
        lenmap[ e ] = w;
    }
    else if ( sscanf( str, "%d", &w ) == 1 ) { 
        lenmap[ e ] = w;
    }
    else return false;

    return true;
}

// read an weighted graph in GML format and 
// find a minimum cycle basis. 
// Output the time taken
int main() {

    // initialize Graph
    graph G;
    lenmap.init( G );

    // create parser and read from standard input
    gml_graph parser( G );
    parser.add_edge_rule( get_edge_weight, gml_string, "label" );
    if ( parser.parse( std::cin ) == false ) return -1;

    // copy from map to edge array
    edge e;
    edge_array<int> len( G, 1 );
    forall_edges( e, G )
        len[ e ] = lenmap [ e ];

    // execute
    float T;
    leda::used_time( T ); // start time

    mcb::edge_num enumb( G );
    array< mcb::spvecfp > mcb;
    array< mcb::spvecfp > proof;

    double errorp = 0.5;
    int w = mcb::DIR_MIN_CYCLE_BASIS<int>( G, \
            len, mcb, proof, enumb, errorp );

    T = used_time( T ); // finish time

    if ( DMCB_verify_basis( G, enumb, mcb, proof ) == false ) 
        leda::error_handler(999,"MIN_CYCLE_BASIS: result is not a cycle basis");

    std::cout << "weight: " << w << " time: " << T << std::endl;

    return 0;
}

/* ex: set ts=8 sw=4 sts=4 noet: */


