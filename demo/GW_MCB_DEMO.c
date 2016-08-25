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
#include <LEP/mcb/mcb.h>
#include <LEP/mcb/determinant.h>
#include <LEP/mcb/verify.h>
#include <LEP/mcb/util.h>

#ifdef LEDA_GE_V5
#include <LEDA/numbers/integer.h>
#include <LEDA/graphics/graphwin.h>
#include <LEDA/graphics/gml_gwgraph.h>
#include <LEDA/core/h_array.h>
#else
#include <LEDA/integer.h>
#include <LEDA/graphwin.h>
#include <LEDA/gml_gwgraph.h>
#include <LEDA/h_array.h>
#endif

#if defined(LEDA_NAMESPACE)
using namespace leda;
#endif

void print_usage(const char * program)
{
    std::cout << "Usage: "<< program <<" [-g] [-w] [-p] [-q] [-r] [-d] [-k value] [-e value]" << std::endl;
    std::cout << std::endl;
    std::cout << "Allows the user to iterative draw a graph and visualize a Minimum Cycle Basis." << std::endl;
    std::cout << std::endl;
    std::cout << "The user can select unweighted or weighted, undirected and directed graphs." << std::endl; 
    std::cout << "The cycle basis can be minimum or an approximation." << std::endl; 
    std::cout << std::endl;
    std::cout << "Inside the window, red edges denote the edges of an arbitrary spanning tree. All" << std::endl;
    std::cout << "remaining edges are black. Each edge has a label. If the graph is unweighted, the" << std::endl;
    std::cout << "label of each edge is a unique identifier. If the graph is weighted, the label is" << std::endl;
    std::cout << "the unique identifer followed by the weight inside parentheses." << std::endl;
    std::cout << "In the case of weighted edges, a slider allows the user to iteratively change the" << std::endl;
    std::cout << "edge weight inside the range [1,99]. The initial edge weight is a random number" << std::endl;
    std::cout << "inside the range [1,10]." << std::endl;
    std::cout << std::endl;
    std::cout << "The menu \"MCB\" is used to compute an MCB and the menu \"Cycles\" can be used to iterate" << std::endl;
    std::cout << "over the cycles of the computed MCB. The cycle edges are denoted by thicker segments." << std::endl;
    std::cout << "Alternatively, various information is printed in the standard output, see the options" << std::endl;
    std::cout << "below." << std::endl;
    std::cout << std::endl;
    std::cout << "A graph can be either constructed iteratively in the window, or can be read from the" << std::endl;
    std::cout << "standard input in GML format, see the options below." << std::endl;
    std::cout << std::endl;
    std::cout <<"-g" << std::endl;
    std::cout <<"          Read GML graph from standard input." << std::endl;
    std::cout <<"-d" << std::endl;
    std::cout <<"          Assume directed graph." << std::endl;
    std::cout <<"-w" << std::endl;
    std::cout <<"          Assume weighted graph." << std::endl;
    std::cout <<"-p" << std::endl;
    std::cout <<"          Do not print the MCB at the standard output." << std::endl;
    std::cout <<"-q" << std::endl;
    std::cout <<"          Print a histogram of the cycles of the MCB at the standard output." << std::endl;
    std::cout <<"-r" << std::endl;
    std::cout <<"          Compute and print the determinant of the cycle basis." << std::endl;
    std::cout <<"-m" << std::endl;
    std::cout <<"          Print the cycle matrix." << std::endl;
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

// global variables
graph G;
edge_map<int> Gcost(G);

array< mcb::spvecfp > mcb_dir;
array< mcb::spvecgf2 > mcb_undir;
int drawc;

mcb::edge_num enumb;
bool computed;
bool directed;
bool directedfp;
bool weighted;
bool printmcb;
bool printdet;
bool printcyclematrix;
bool printhist;
bool approx;
double errorp;
int k;
leda::integer prime;

const int CYCLE_EDGE_WIDTH = 3;
const int DEFAULT_EDGE_WIDTH = 1;

// output functions
template<class Container>
void print_cycle_basis( const array<Container>& mcb, int w, const mcb::edge_num& enumb ); 

template<>
void print_cycle_basis<mcb::spvecfp>( const array<mcb::spvecfp>& mcb, int w, const mcb::edge_num& enumb)
{
    int N = enumb.dim_cycle_space();
    if ( N > 0 ) {
        std::cout << "*** DIRECTED MINIMUM CYCLE BASIS ***" << std::endl;
        if ( approx ) 
            std::cout << 2*k-1 << " approximation" << std::endl;
        if ( directedfp )
            std::cout << "minimum over F_" << prime << std::endl;

        for( int i = 0; i < N; i++ ) {
            std::cout << "cycle " << i+1;
            std::cout << " : "; 
            
            list_item it = mcb[i].first();
            while( it != nil ) { 
                std::cout << ( ( mcb[i].inf( it ) == -1 )? "(-)":"" );
                std::cout << mcb[i].index( it ); 

                it = mcb[i].succ( it );
                if ( it != nil ) std::cout << " ";
            }
            std::cout  << std::endl;
        }
        std::cout << "Total weight = " << w << std::endl;
    }
    else {
        std::cout << "Dimension of cycle space is zero" << std::endl;
    }
}

template<>
void print_cycle_basis<mcb::spvecgf2>( const array<mcb::spvecgf2>& mcb, int w, const mcb::edge_num& enumb)
{
    int N = enumb.dim_cycle_space();
    if ( N > 0 ) {
        std::cout << "*** MINIMUM CYCLE BASIS ***" << std::endl;
        if ( approx ) 
            std::cout << 2*k-1 << " approximation" << std::endl;
        if ( directedfp )
            std::cout << "minimum over F_" << prime << std::endl;
        for( int i = 0; i < N; i++ ) {
            std::cout << "cycle " << i+1 << " : " << mcb[i] << std::endl; 
        }
        std::cout << "Total weight = " << w << std::endl;
    }
    else {
        std::cout << "Dimension of cycle space is zero" << std::endl;
    }
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

// read edge weight
// in the following form: label "weight" or label "uniquenumber (weight)"
bool get_edge_weight( const gml_object* gobj, graph* G, edge e ) {
    char* str = gobj->get_string();
    int w, tw;
    if ( sscanf( str, "%d (%d)", &tw, &w ) == 2 ) {
        Gcost[ e ] = w;
    }
    else if ( sscanf( str, "%d", &w ) == 1 ) {
        Gcost[ e ] = w;
    }
    else 
        return false;

    return true;
}

void run_and_display(GraphWin& gw) {
    if ( approx ) {
        if ( directedfp ) 
            gw.message( string("\\bf Approximate MCB over F_%s", prime.to_string() ).c_str() );
        else
            gw.message("\\bf Approximate MCB");
    }
    else { 
        if ( directedfp )
            gw.message( string("\\bf MCB over F_%s", prime.to_string() ).c_str() );
        else 
            gw.message("\\bf MCB");
    }

    if ( directed || directedfp ) 
    {
        array< mcb::spvecfp > proof;
        int w; 

        if ( directedfp ) 
            w = mcb::DMCB_APPROX( G, Gcost, k, mcb_dir, enumb, prime );
        else 
            w = mcb::DMCB_APPROX( G, Gcost, k, mcb_dir, enumb, errorp );

        if ( mcb::verify_cycle_basis( G, enumb, mcb_dir ) == false ) 
            leda::error_handler(999,"MIN_CYCLE_BASIS: result is not a cycle basis");

        if ( printmcb )
            print_cycle_basis( mcb_dir, w, enumb );
    
        if ( ( printdet || printcyclematrix) && enumb.dim_cycle_space() > 0 ) { 
            leda::integer_matrix B( enumb.dim_cycle_space(), enumb.dim_cycle_space() );
            cycle_matrix( G, mcb_dir, enumb, B );
            if ( printcyclematrix ) { 
                std::cout << "cycle matrix B = " << std::endl;
                mcb::output_maple_format( std::cout, B );
            }
            if ( printdet )
                std::cout << "det(B) = " << abs( determinant( B ) ) << std::endl;
        }

        if ( printhist )
            print_histogram( G, Gcost, mcb_dir, enumb );
    }
    else { 
        int w;
        w = mcb::UMCB_APPROX( G, Gcost, k, mcb_undir, enumb );

        if ( mcb::verify_cycle_basis( G, enumb, mcb_undir ) == false ) 
            leda::error_handler(999,"MIN_CYCLE_BASIS: result is not a cycle basis");

        if ( printmcb )
            print_cycle_basis( mcb_undir, w, enumb );

        if ( ( printdet || printcyclematrix) && enumb.dim_cycle_space() > 0 ) { 
            leda::integer_matrix B( enumb.dim_cycle_space(), enumb.dim_cycle_space() );
            cycle_matrix( G, mcb_undir, enumb, B );
            if ( printcyclematrix ) { 
                std::cout << "cycle matrix B = " << std::endl;
                mcb::output_maple_format( std::cout, B );
            }
            if ( printdet )
                std::cout << "det(B) = " << abs( determinant( B ) ) << std::endl;
        }

        if ( printhist )
            print_histogram( G, Gcost, mcb_undir, enumb );
    }
    computed = true;
}

void draw_graph( GraphWin &gw )
{
    bool flush = gw.set_flush(false);
    edge e;
    forall_edges(e,G) {
        if ( enumb.tree(e) )
            gw.set_color( e, red );
        else
            gw.set_color( e, black );
        if ( weighted ) {
            if ( Gcost[e] == 0 ) { 
                Gcost[e] = rand_int(1, 10);
            }
            gw.set_slider_value(e,Gcost[e]/100.0,1);
            gw.set_label(e,string("%d (%d)", enumb(e), Gcost[e] ));
        }
        else {
            Gcost[e] = 1;
            gw.set_label(e,string("%d", enumb(e) ));
        }
        gw.set_width( e, DEFAULT_EDGE_WIDTH );
    }
    gw.set_flush(flush);
    gw.redraw();
}

void init_graph_handler( GraphWin &gw ) {
    Delete_Loops( G );
    gw.update_graph();
    enumb = mcb::edge_num( G );
    draw_graph( gw );
    drawc = -1;
    computed = false;
}

void new_edge_handler(GraphWin& gw, edge e) {
    init_graph_handler( gw );
}

void del_edge_handler(GraphWin& gw) {
    init_graph_handler( gw );
}

void new_node_handler(GraphWin& gw, node) {
    init_graph_handler( gw );
}

void del_node_handler(GraphWin& gw) {
    init_graph_handler( gw );
}

// rank sliders
void start_cost_slider_handler(GraphWin& gw, edge, double)
{
    gw.message("\\bf\\red Changing edge weight");
}

void cost_slider_handler(GraphWin& gw, edge e, double f)
{
    bool changed = false; 
    if ( int(100*f) != Gcost[e] ) 
        changed = true;
    if ( changed ) { 
        Gcost[e] = int(100*f);
        if( Gcost[e] == 0 ) 
            Gcost[e] = 1;
        if( weighted )
            gw.set_label(e,string("%d (%d)", enumb(e), Gcost[e] ));
        else 
            gw.set_label(e,string("%d", enumb(e) ));
        computed = false;
    }
}

void end_cost_slider_handler(GraphWin& gw, edge, double) {}

void highlight_cycle(GraphWin& gw) {
    int c = drawc;

    if ( c < 0 || c >= enumb.dim_cycle_space() )
        return;

    bool flush = gw.set_flush(false);

    edge e;
    forall_edges(e,G) {
        gw.set_width( e, DEFAULT_EDGE_WIDTH );
    }

    int w = 0;
    if ( directed || directedfp )
    {
        list_item it = mcb_dir[c].first();
        while( it != nil ) { 
            e = enumb( mcb_dir[c].index(it) );
            w += Gcost[e];
            gw.set_width( e , CYCLE_EDGE_WIDTH );
            it = mcb_dir[c].succ( it );
        }
    }
    else { 
        int j = 0;
        forall(j, mcb_undir[c]) {
            e = enumb(j);
            w += Gcost[e];
            gw.set_width( e , CYCLE_EDGE_WIDTH );
        }
    }

    std::ostringstream stm;
    stm << w;
    std::string s = "\\bf Weight = " + std::string(stm.str()) + " .";
    gw.message( s.c_str() );

    gw.set_flush(flush);
    gw.redraw();
}

void highlight_prev_cycle(GraphWin& gw) { 
    if ( !computed )
        run_and_display( gw );
    if ( enumb.dim_cycle_space() == 0 ) 
        return;
    if ( --drawc < 0 )
        drawc = enumb.dim_cycle_space() - 1;
    highlight_cycle( gw );
}

void highlight_next_cycle(GraphWin& gw) {
    if ( !computed ) 
        run_and_display( gw );
    if ( enumb.dim_cycle_space() == 0 ) 
        return;
    if ( ++drawc >= enumb.dim_cycle_space() )
        drawc = 0;
    highlight_cycle( gw );
}

void highlight_first_cycle(GraphWin& gw) {
    if ( !computed )
        run_and_display( gw );
    if ( enumb.dim_cycle_space() == 0 ) 
        return;
    drawc = 0;
    highlight_cycle( gw );
}


void setup_menus( GraphWin &gw ) {
    int mcbmenu = gw.add_menu("MCB");
    gw.add_simple_call( run_and_display, "Compute", mcbmenu );    

    drawc = -1;
    int cmenu = gw.add_menu( "Cycles" );
    gw.add_simple_call( highlight_first_cycle, "First", cmenu );
    gw.add_simple_call( highlight_prev_cycle, "Previous", cmenu );
    gw.add_simple_call( highlight_next_cycle, "Next", cmenu );
}

int main(int argc, char* argv[]) {

    computed = false;
    weighted = false;
    directed = false;
    directedfp = false;
    printmcb = true;
    printdet = false;
    printcyclematrix = false;
    printhist = false;
    approx = false;
    errorp = 0.1;
    bool readgml = false;
    k = 1;
    int c;

    opterr = 0;

    while ((c = getopt (argc, argv, "gwcmprdk:qhe:f:")) != -1)
        switch (c)
        {
            case 'g':
                readgml = true;
                break;
            case 'd':
                directed = true;
                break;
            case 'w':
                weighted = true;
                break;
            case 'p':
                printmcb = false;
                break;
            case 'r':
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
                print_usage( argv[0] );
                exit( EXIT_SUCCESS );
            default:
                std::cout << "Unknown option, try -h for help." << std::endl;
                break;
        }

    GraphWin gw(G,800,600,"Minimum Cycle Basis Demo");

    if ( readgml ) { 
        gml_gwgraph parser( &gw );
        if ( weighted )  
            parser.add_edge_rule( get_edge_weight, gml_string, "label" );
        if ( parser.parse( std::cin ) == false ) {
            std::cerr << "Error parsing GML graph from standard input, aborting.." << std::endl;
            abort();
        }
        init_graph_handler( gw );
    }

    setup_menus( gw );

    gw.display( window::center, window::center );
    if ( readgml )
        gw.zoom_graph();
    gw.set_action(A_LEFT | A_DRAG | A_EDGE , NULL);
    gw.set_init_graph_handler(init_graph_handler);
    gw.set_new_node_handler(new_node_handler);
    gw.set_del_node_handler(del_node_handler);
    gw.set_new_edge_handler(new_edge_handler);
    gw.set_del_edge_handler(del_edge_handler);
    if ( weighted ) {
        gw.set_start_edge_slider_handler(start_cost_slider_handler,1);
        gw.set_edge_slider_handler(cost_slider_handler,1);
        gw.set_end_edge_slider_handler(end_cost_slider_handler,1);
        gw.set_edge_slider_color(blue,1);
    }
    gw.set_directed( directed || directedfp );
    gw.edit();

    return 0;
}

/* ex: set ts=4 sw=4 sts=4 et: */
