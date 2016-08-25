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
#include <LEP/mcb/min_cycle_basis.h>

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



int main() { 
        int i = 0, n, m;

        while( true ) { 
                std::cout << "\nstarting example " << i++;
                std::cout.flush();

                // create random graph
                graph G;
                edge_array<int> len;

                n = rand_int( 1, 70 );
                m = rand_int( 1, n*(n-1)/2 );
                create_graph( n, m, G, len );

                mcb::edge_num enumb( G );
                int N = enumb.dim_cycle_space();
                int c = enumb.num_weak_connected_comp();

                std::cout << " N = m - n + c = " << G.number_of_edges(); 
                std::cout << " - " << G.number_of_nodes() << " + ";
                std::cout << c << " = " << N;

                if ( N <= 0 ) continue;

                // MCB
                array< d_int_set > mcb;
                array< d_int_set > proof;
                int w = mcb::MIN_CYCLE_BASIS_DEPINA( G, len, mcb, proof, enumb );
                if ( N > 0 ) { 
                        std::cout << "  - MCB weight: " << w;
                        std::cout.flush();
                }

                // verify minimum cycle basis
                if ( ! mcb::MIN_CYCLE_BASIS_DEPINA_CHECK( G, len, mcb, proof, enumb ) )
                        error_handler(999,"MIN_CYCLE_BASIS:\
                        not a minimum cycle basis :(");

                // now try the sparse vector representation of de Pina
                array< mcb::spvecgf2 > mcbSP; 
                array< mcb::spvecgf2 > proofSP;
                int wSP = mcb::MIN_CYCLE_BASIS_DEPINA( G, len, mcbSP, proofSP, enumb );
                assert( w == wSP );
                if ( N > 0 ) { 
                    std::cout << " - DEPINASP(MCB) = " << wSP; 
                    std::cout.flush();
                }
                // verify minimum cycle basis
                if ( ! mcb::MIN_CYCLE_BASIS_DEPINA_CHECK( G, len, mcbSP, proofSP, enumb ) )
                        error_handler(999,"MIN_CYCLE_BASIS:\
                        not a minimum cycle basis :(");
    
                // now try the hybrid-superset algorithm
                array< d_int_set > mcbh, proofh;
                int wh = mcb::MIN_CYCLE_BASIS_HYBRID( G, len, mcbh, proofh, enumb );
                if ( N > 0 ) { 
                        std::cout << " - HYBRID(MCB) = " << wh;
                        std::cout.flush();
                }

                // test copy-constructor of edge_num
                mcb::edge_num enumb1 = enumb ;

                // verify minimum cycle basis
                if ( ! mcb::MIN_CYCLE_BASIS_DEPINA_CHECK( G, len, mcbh, proofh, enumb ) )
                        error_handler(999,"MIN_CYCLE_BASIS (HYBRID):\
                        not a minimum cycle basis :(");
                assert( w == wh );


                // now try different modes
                array< list<edge> > mcb1, mcb12, proof12, mcb13, mcb14, proof14;
                int w1 = mcb::MIN_CYCLE_BASIS_DEPINA( G, len, mcb1 );
                int w2 = mcb::MIN_CYCLE_BASIS_DEPINA( G, len, mcb12, proof12 );
                int w11 = mcb::MIN_CYCLE_BASIS_HYBRID( G, len, mcb13 );
                int w12 = mcb::MIN_CYCLE_BASIS_HYBRID( G, len, mcb14, proof14); 
                assert( w == w1 && w1 == w2 && w11 == w2 && w12 == w11 );

                // check CHECK with lists
                assert( mcb::MIN_CYCLE_BASIS_DEPINA_CHECK( G, len, mcb12, proof12 ) );

                // now try with unweighted graphs
                array< d_int_set > mcb_un, proof_un;
                int w5 = mcb::MIN_CYCLE_BASIS_DEPINA( G, mcb_un, proof_un, enumb );
                std::cout << " Unweighted MCB = " << w5;

                // and list mode for unweights graphs
                array< list<edge> > mcb6, mcb71, mcb72;
                array< d_int_set > mcb_h1, proof_h1;
                array< list<edge> > mcbh6, mcbh71, mcbh72;
                int w6 = mcb::MIN_CYCLE_BASIS_DEPINA( G, mcb6 );
                int w7 = mcb::MIN_CYCLE_BASIS_DEPINA( G, mcb71, mcb72 );
                int w8 = mcb::MIN_CYCLE_BASIS_HYBRID( G, mcb_h1, proof_h1, enumb );
                int w9 = mcb::MIN_CYCLE_BASIS_HYBRID( G, mcb_h1, enumb );
                int w10 = mcb::MIN_CYCLE_BASIS_HYBRID( G, mcbh6 );
                int w15 = mcb::MIN_CYCLE_BASIS_HYBRID( G, mcbh71, mcbh72 );

                assert( w6 == w7 && w5 == w6 && w8 == w9 && w10 == w15 && 
                w9 == w10 && w7 == w8 );

        }
        std::cout << "\n" << std::endl;

        return 0;
}

/* ex: set ts=8 sw=4 sts=4 noet: */



