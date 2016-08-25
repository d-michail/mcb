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

#include <iostream>
#include <stdio.h>
#include <sys/resource.h>
#include <LEP/mcb/determinant.h>
#include <LEP/mcb/mcb.h>
#include <LEP/mcb/verify.h>

#ifdef LEDA_GE_V5
#include <LEDA/graph/graph_misc.h>
#include <LEDA/graph/gml_graph.h>
#include <LEDA/graph/edge_map.h>
#include <LEDA/core/h_array.h>
#else
#include <LEDA/graph_misc.h>
#include <LEDA/gml_graph.h>
#include <LEDA/edge_map.h>
#include <LEDA/h_array.h>
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

void print_usage(const char * program)
{
    std::cout << "Usage: "<< program <<" [-w] [-s] [-c] [-f] [-p] [-q] [-d] [-k value] [-e value]" << std::endl;
    std::cout << "Read a GML graph from standard input and compute its MCB." << std::endl;
    std::cout << std::endl;
    std::cout << "Without any options the program selects the best algorithm." << std::endl;
    std::cout << "Based on whether the graph is directed or undirected the program" << std::endl;
    std::cout << "selects the appropriate method." << std::endl;
    std::cout << std::endl;
    std::cout <<"-w" << std::endl;
    std::cout <<"          Assume weighted graph. The gml file should contain label entries for each edge." << std::endl;
    std::cout <<"          Each label entry should be either \"weight\" or \"uniquenumber (weight)\"" << std::endl;
    std::cout <<"-s" << std::endl;
    std::cout <<"          Use the support vector approach (undirected graphs)." << std::endl;
    std::cout <<"-c" << std::endl;
    std::cout <<"          Use the hybrid approach (undirected graphs)." << std::endl;
    std::cout <<"-l" << std::endl;
    std::cout <<"          Use the hybrid approach with labelled trees (undirected graphs)." << std::endl;
    std::cout <<"-p" << std::endl;
    std::cout <<"          Print the MCB at the standard output." << std::endl;
    std::cout <<"-m" << std::endl;
    std::cout <<"          Print the cycle matrix." << std::endl;
    std::cout <<"-q" << std::endl;
    std::cout <<"          Print a histogram of the cycles of the MCB at the standard output." << std::endl;
    std::cout <<"-d" << std::endl;
    std::cout <<"          Compute an print the determinant of the cycle basis." << std::endl;
    std::cout <<"-k value" << std::endl;
    std::cout <<"          If this option is given, we compute an approximate MCB." << std::endl;
    std::cout <<"          The approximation factor will be 2k-1." << std::endl;
    std::cout <<"-e value" << std::endl;
    std::cout <<"          Probability of error in the case of a directed graph. This is the" << std::endl;
    std::cout <<"          probability that the returned basis is not minimum." << std::endl;
    std::cout <<"          Default value is 0.375." << std::endl;
    std::cout <<"-f value" << std::endl;
    std::cout <<"          Try to compute a directed minimum cycle basis in F_p where p is the value " << std::endl;
    std::cout <<"          parameter. This is not necessarily a minimum cycle basis." << std::endl;
    std::cout <<"          The value parameter must be a prime number." << std::endl;
    std::cout <<"          If this option is given, the option -e is ignored." << std::endl;
    std::cout <<"-h" << std::endl;
    std::cout <<"          Display this message." << std::endl;
    std::cout << std::endl;
    std::cout << "Report bugs to <dmixdim@googlemail.com>." << std::endl;
}

template<class W, class Container>
void print_histogram( const graph& G, const edge_array<W>& len, const array<Container>& mcb, const mcb::edge_num& enumb )
{
    h_array< W ,int > sizes;
    W s;
    mcb::ComputeWeight<W,Container> accu( G, len, enumb );
    for( int i = 0; i < enumb.dim_cycle_space(); ++i )
    {
        s = accu( mcb[i] );
        if ( sizes.defined( s ) )
            sizes[ s ] = sizes[ s ] + 1;
        else
            sizes[ s ] = 1;
    }

    forall_defined( s, sizes )
        std::cout << "length " << s << " - number of cycles " << sizes[s] << std::endl;
}

template<class Container>
void print_det_and_matrix( const graph& G, const array<Container>& mcb, const mcb::edge_num& enumb, 
                           bool printdet, bool printcyclematrix )
{
    if ( ( printdet || printcyclematrix) && enumb.dim_cycle_space() > 0 ) {
        leda::integer_matrix B( enumb.dim_cycle_space(), enumb.dim_cycle_space() );
        cycle_matrix( G, mcb, enumb, B );
        if ( printcyclematrix ) {
            std::cout << "cycle matrix B = " << std::endl;
            mcb::output_maple_format( std::cout, B );
        }
        if ( printdet )
            std::cout << "det(B) = " << abs( determinant( B ) ) << std::endl;
    }
}

int main(int argc, char* argv[]) {

    bool support = true; 
    bool hybrid = false;
    bool weighted = false;
    bool treeshybrid = false;
    bool printmcb = false;
    bool printdet = false;
    bool printcyclematrix = false;
    bool printhist = false;
    bool approx = false;
    bool directedfp = false;
    double errorp = 0.1;
    leda::integer prime;
    int k;
    int c;

    opterr = 0;

    while ((c = getopt (argc, argv, "wsmcf:lpdk:qhe:")) != -1)
        switch (c)
        {
            case 'w':
                weighted = true;
                break;
            case 's':
                support = true; 
                hybrid = false;
                treeshybrid = false;
                break;
            case 'c':
                support = false;
                hybrid = true;
                treeshybrid = false;
                break;
            case 'l':
                support = false;
                hybrid = false; 
                treeshybrid = true;
                break;
            case 'p':
                printmcb = true;
                break;
            case 'd':
                printdet = true;
                break;
            case 'm':
                printcyclematrix = true;
                break;
            case 'q':
                printhist = true;
                break;
            case 'k':
                k = atoi( optarg );
                if ( k > 1 )
                    approx = true;
                else 
                    k = 1;
                break;
            case 'e':
                errorp = atof( optarg );
                if ( errorp <= 0.0 || errorp > 1.0 )
                    errorp = 0.1;
                break;
            case 'f': 
                prime = atoi( optarg );
                if ( prime < 3 || ! mcb::primes<mcb::ptype>::is_prime( prime ) ) {
                    std::cout << "-f parameter value should be a prime > 2, ignoring it.." << std::endl;   
                }
                else 
                    directedfp = true;
                break;
            case 'h':
            default:
                print_usage( argv[0] );
                exit( EXIT_SUCCESS );
        }

    // initialize Graph
    graph G;
    lenmap.init( G );

    // create parser and read from standard input
    gml_graph parser( G );
    if ( weighted )
        parser.add_edge_rule( get_edge_weight, gml_string, "label" );
    if ( parser.parse( std::cin ) == false ) { 
        std::cerr << "Error parsing GML file, aborting.." << std::endl;
        abort();
    }

    // copy from map to edge array
    edge e;
    edge_array<int> len( G, 1 );
    if ( weighted )
        forall_edges( e, G )
            len[ e ] = lenmap [ e ];

    // execute
    float T;
    leda::used_time( T ); // start time

    mcb::edge_num enumb( G );
    int w;

    if ( G.is_undirected() )
    {
        if ( approx ) { 
            std::cout << "Computing undirected " << 2*k-1 << "-MCB with default approach." << std::endl;
            array< mcb::spvecgf2 > mcb;
            w = mcb::UMCB_APPROX( G, len, k, mcb, enumb );
            print_det_and_matrix( G, mcb, enumb, printdet, printcyclematrix );
            if (printmcb)
                for( int i = 0; i < enumb.dim_cycle_space(); ++i )
                    std::cout << "cycle " << i+1 << " : " << mcb[i] << std::endl;
            if (printhist)
                print_histogram( G, len, mcb, enumb );
        }
        else if ( support ) { 
            std::cout << "Computing undirected MCB with Support Vector approach." << std::endl;
            array< mcb::spvecgf2 > mcb;
            array< mcb::spvecgf2 > proof;
            w = mcb::UMCB_SVA( G, len, mcb, proof, enumb );
            print_det_and_matrix( G, mcb, enumb, printdet, printcyclematrix );
            if (printmcb)
                for( int i = 0; i < enumb.dim_cycle_space(); ++i )
                    std::cout << "cycle " << i+1 << " : " << mcb[i] << std::endl;
            if (printhist)
                print_histogram( G, len, mcb, enumb );
        }
        else if ( hybrid ) { 
            std::cout << "Computing undirected MCB with Hybrid approach." << std::endl;
            array< leda::d_int_set > mcb;
            array< leda::d_int_set > proof;
            w = mcb::UMCB_HYBRID( G, len, mcb, proof, enumb );
            print_det_and_matrix( G, mcb, enumb, printdet, printcyclematrix );
            if (printmcb)
                for( int i = 0; i < enumb.dim_cycle_space(); ++i )
                    std::cout << "cycle " << i+1 << " : " << mcb[i] << std::endl;
            if (printhist)
                print_histogram( G, len, mcb, enumb );
        }
        else if ( treeshybrid ) { 
            std::cout << "Computing undirected MCB with Hybrid + Labelled Trees approach." << std::endl;
            array< mcb::spvecgf2 > mcb;
            array< mcb::spvecgf2 > proof;
            w = mcb::UMCB_FH( G, len, mcb, enumb );
            print_det_and_matrix( G, mcb, enumb, printdet, printcyclematrix );
            if (printmcb)
                for( int i = 0; i < enumb.dim_cycle_space(); ++i )
                    std::cout << "cycle " << i+1 << " : " << mcb[i] << std::endl;
            if (printhist)
                print_histogram( G, len, mcb, enumb );
        }
        else { 
            std::cout << "Computing undirected MCB with default approach." << std::endl;
            array< mcb::spvecgf2 > mcb;
            array< mcb::spvecgf2 > proof;
            w = mcb::UMCB( G, len, mcb, enumb );
            print_det_and_matrix( G, mcb, enumb, printdet, printcyclematrix );
            if (printmcb)
                for( int i = 0; i < enumb.dim_cycle_space(); ++i )
                    std::cout << "cycle " << i+1 << " : " << mcb[i] << std::endl;
            if (printhist)
                print_histogram( G, len, mcb, enumb );
        }
    }
    else 
    { 
        if ( approx ) 
            std::cout << "Computing directed " << 2*k-1 << "-MCB with default approach." << std::endl;
        else 
            std::cout << "Computing directed MCB with default approach." << std::endl;
        if ( directedfp )
            std::cout << "Computation is over F_" << prime << " instead of Q" << std::endl;

        array< mcb::spvecfp > mcb;
        array< mcb::spvecfp > proof;

        if ( approx ) { 
            if ( directedfp )
                w = mcb::DMCB_APPROX( G, len, k, mcb, enumb, prime );
            else
                w = mcb::DMCB_APPROX( G, len, k, mcb, enumb, errorp );
        }
        else { 
            if ( directedfp )
                w = mcb::DMCB<int>( G, len, mcb, proof, enumb, prime );
            else
                w = mcb::DMCB<int>( G, len, mcb, proof, enumb, errorp );
        }

        print_det_and_matrix( G, mcb, enumb, printdet, printcyclematrix );

        if ( printmcb )
            for( int i = 0; i < enumb.dim_cycle_space(); ++i ) {
                std::cout << "cycle " << i+1 << " : ";

                list_item it = mcb[i].first();
                while( it != nil ) {
                    std::cout << ( ( mcb[i].inf( it ) == -1 )? "(-)":"" );
                    std::cout << mcb[i].index( it );

                    it = mcb[i].succ( it );
                    if ( it != nil ) std::cout << " ";
                }
                std::cout << std::endl;
            }

        if (printhist)
            print_histogram( G, len, mcb, enumb );
    }

    T = used_time( T ); // finish time

    //if ( mcb::verify_cycle_basis( G, enumb, mcb ) == false ) 
      //  leda::error_handler(999,"MIN_CYCLE_BASIS: result is not a cycle basis");

    std::cout << "weight: " << w << std::endl;
    std::cout << "time to compute: " << T << std::endl;

    return 0;
}

/* ex: set ts=4 sw=4 sts=4 et: */


