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
#include <LEP/mcb/dir_min_cycle_basis.h>

#ifdef LEDA_GE_V5
#include <LEDA/graphics/graphwin.h>
#else
#include <LEDA/graphwin.h>
#endif

#if defined(LEDA_NAMESPACE)
using namespace leda;
#endif

graph G;
edge_map < int >Gcost( G );

void run_and_display( GraphWin & gw )
{

    bool flush = gw.set_flush( false );

    gw.message( "\\bf Computing Directed Minimum Cycle Basis" );

    edge_array < int >cost( G );
    node v;
    edge e;

    forall_edges( e, G ) cost[e] = Gcost[e];

    // meet preconditions
    Delete_Loops( G );
    Make_Simple( G );
    gw.update_graph(  );

    // initialize
    mcb::edge_num enumb( G );
    int c = enumb.num_weak_connected_comp(  );
    int N = enumb.dim_cycle_space(  );

    array < mcb::spvecfp > mcb;
    array < mcb::spvecfp > proof;
    double errorp = 0.1;

    int w = mcb::DIR_MIN_CYCLE_BASIS < int >( G,
					      cost, mcb, proof, enumb,
					      errorp );

    if ( DMCB_verify_basis( G, enumb, mcb, proof ) == false )
	leda::error_handler( 999,
			     "MIN_CYCLE_BASIS: result is not a cycle basis" );

    // assign labels to edges: num( weight )
    forall_edges( e, G ) {
	gw.set_color( e, black );
	gw.set_label( e, string( "%d (%d)", enumb( e ), cost[e] ) );
    }

    // set edges of spanning tree as red
    forall_edges( e, G )
	if ( enumb.tree( e ) )
	gw.set_color( e, red );

    if ( N > 0 ) {
	// print cycles
	std::cout << "*** DIRECTED MINIMUM CYCLE BASIS ***" << std::endl;
	std::cout << "Total weight = " << w << std::endl;
	for ( int i = 0; i < N; i++ ) {
	    std::cout << "cycle " << i + 1;
	    std::cout << " : ";

	    list_item it = mcb[i].first(  );

	    while ( it != nil ) {
		std::cout << ( ( mcb[i].inf( it ) == -1 ) ? "(-)" : "" );
		std::cout << mcb[i].index( it ) - 1;

		it = mcb[i].succ( it );
		if ( it != nil )
		    std::cout << " ";
	    }

	    std::cout << std::endl;
	}
    }

    gw.set_flush( flush );
    gw.redraw(  );
}

void new_node_handler( GraphWin & gw, node )
{
}

void new_edge_handler( GraphWin & gw, edge e )
{
    Gcost[e] = rand_int( 1, 10 );
    gw.set_slider_value( e, Gcost[e] / 100.0, 1 );
    run_and_display( gw );
}

void del_edge_handler( GraphWin & gw )
{
    run_and_display( gw );
}

void del_node_handler( GraphWin & gw )
{
    run_and_display( gw );
}

void init_graph_handler( GraphWin & gw )
{
    edge e;

    forall_edges( e, G ) {
	if ( Gcost[e] == 0 ) {
	    Gcost[e] = rand_int( 1, 10 );
	}
	gw.set_slider_value( e, Gcost[e] / 100.0, 1 );
    }
    run_and_display( gw );
}

// rank sliders
void start_cost_slider_handler( GraphWin & gw, edge, double )
{
    gw.message( "\\bf\\red Change Edge Cost" );
}

void cost_slider_handler( GraphWin & gw, edge e, double f )
{
    Gcost[e] = int ( 100 * f );

    if ( Gcost[e] == 0 )
	Gcost[e] = 1;
}

void end_cost_slider_handler( GraphWin & gw, edge, double )
{
    run_and_display( gw );
}

int main(  )
{
    GraphWin gw( G, "Directed Minimum Cycle Basis" );

    gw.add_help_text( "DIR_MIN_CYCLE_BASIS" );
    gw.display(  );
    gw.display_help_text( "DIR_MIN_CYCLE_BASIS" );

    gw.set_action( A_LEFT | A_DRAG | A_EDGE, NULL );
    gw.win_init( 0, 200, 0 );

    // set handlers
    gw.set_init_graph_handler( init_graph_handler );

    gw.set_del_edge_handler( del_edge_handler );
    gw.set_del_node_handler( del_node_handler );
    gw.set_new_edge_handler( new_edge_handler );

    gw.set_start_edge_slider_handler( start_cost_slider_handler, 1 );
    gw.set_edge_slider_handler( cost_slider_handler, 1 );
    gw.set_end_edge_slider_handler( end_cost_slider_handler, 1 );
    gw.set_edge_slider_color( blue, 1 );

    gw.set_directed( true );

    gw.edit(  );

    return 0;
}

/* ex: set ts=8 sw=4 sts=4 noet: */