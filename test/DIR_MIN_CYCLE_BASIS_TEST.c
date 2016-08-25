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
// Copyright (C) 2004-2006 - Dimitrios Michail


#include <iostream>
#include <LEP/mcb/dir_min_cycle_basis.h>
#include <LEP/mcb/verify.h>

#ifdef LEDA_GE_V5
#include <LEDA/graph/graph.h>
#include <LEDA/graph/graph_misc.h>
#include <LEDA/graph/graph_gen.h>
#include <LEDA/graph/edge_array.h>
#include <LEDA/system/assert.h>
#else
#include <LEDA/graph.h>
#include <LEDA/graph_misc.h>
#include <LEDA/graph_gen.h>
#include <LEDA/edge_array.h>
#include <LEDA/assert.h>
#endif

#if defined(LEDA_NAMESPACE)
using namespace leda;
#endif

// edges lengths
edge_array<int> len;


// write edge weight to ostream
void write_edge_weight( std::ostream& o, const graph* G, const edge e ) { 
    o << "    label \"" << len[e] << "\"" << std::endl;
}


// create graph
void create_graph( int n, int m, graph& G, edge_array<int>& len ) { 

    // create random graph
    random_simple_loopfree_graph( G, n, m );

    // give non-negative edge lengths
    len.init( G );
    edge e;
    forall_edges (e,G)
        len[e] = rand_int( 0, 65536 );
}



int main() { 

    int i, n, m;

    i = 1;
    while( true ) { 
        std::cout << "\nstarting example " << i++;
        std::cout.flush();

        // create random graph
        graph G;

        n = rand_int( 1, 50 );
        m = rand_int( 1, n*(n-1) );
        create_graph( n, m, G, len );

        // write graph to file
        G.write_gml("graph.input", 0, write_edge_weight );

        mcb::edge_num enumb( G );
        int N = enumb.dim_cycle_space();
        int c = enumb.num_weak_connected_comp();

        std::cout << " N = m - n + c = " << G.number_of_edges();
        std::cout << " - " << G.number_of_nodes() << " + ";
        std::cout << c << " = " << N;
        std::cout.flush();

        if ( N <= 0 ) continue;

        // now run with sparse vectors
        array< mcb::spvecfp > mcb; 
        array< mcb::spvecfp > proof;
        double error = 0.1;
        int w = mcb::DMCB<int>( G, \
                len, mcb, proof, enumb, error );
        assert( verify_cycle_basis( G, enumb, mcb ) );
        if ( N > 0 ) {
            std::cout << " - MCB weight: " << w;
            std::cout.flush();
        }

        // try different modes
/*      {
            array< list<edge> > mcb1;
            int w1 = mcb::DMCB<int>( G, \
                     len, mcb1  );

            array< array<mcb::etype> > mcb2;
            int w2 = mcb::DMCB<int>( G, len, mcb2, enumb );
        } */

        // instantiate with integers
/*      { 
            edge_array<integer> len3 ( G );
            edge e3; 
            forall_edges( e3, G ) len3[ e3 ] = integer::random( 64 );
            array< mcb::spvecfp > mcb3;
            array< mcb::spvecfp > proof3;
            integer w3 = mcb::DMCB<integer>( G, 
                         len3, mcb3, proof3, enumb );
            if ( N > 0 ) {
                std::cout << " - MCB weight: " << w3;
                std::cout.flush();
            }
        }*/
        
    }
}

/* ex: set ts=8 sw=4 sts=4 noet: */


