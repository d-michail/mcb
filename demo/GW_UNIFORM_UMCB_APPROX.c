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
#include <LEDA/graphics/graphwin.h>
#include <LEDA/graph/graph.h>
#else
#include <LEDA/graphwin.h>
#include <LEDA/graph.h>
#endif


#if defined(LEDA_NAMESPACE)
using namespace leda;
#endif

graph G;
int k;

void run_and_display(GraphWin& gw) { 

    bool flush = gw.set_flush(false);

    gw.message("\\bf Computing Approximate Minimum Cycle Basis");

    edge e;


    // meet preconditions
    Delete_Loops( G );
    G.make_undirected();
    gw.update_graph();
    gw.set_flush(flush);
    gw.redraw();

    G.write_gml("graph.gml", 0 );

    // initialize
    mcb::edge_num enumb( G );
    int N = enumb.dim_cycle_space();
    array< mcb::spvecgf2 > mcb;

    int w = mcb::UMCB_APPROX( G, k , mcb, enumb );

    forall_edges(e,G) {
	gw.set_color( e, black );
	gw.set_label(e,string("%d", enumb(e) ));
    }

    // set edges of spanning tree as red
    forall_edges( e , G )
	if ( enumb.tree(e) ) 
	    gw.set_color( e, red );

    if ( N > 0 ) { 
	// print cycles
	std::cout << "*** " << (2*k-1) << "-APPROX";
	std::cout << " MINIMUM CYCLE BASIS ***" << std::endl;
	std::cout << "Total weight = " << w << std::endl;
	for( int i = 0; i < N; i++ ) { 
	    std::cout << "cycle " << i+1; 
	    std::cout << " : " << mcb[i] << std::endl;
	}
    }
    else {
	std::cout << "Dimension of cycle space is zero" << std::endl;
    }

    gw.set_flush(flush);
    gw.redraw();
}

void new_node_handler(GraphWin& gw, node) {}

void new_edge_handler(GraphWin& gw, edge e) {
    run_and_display(gw);
}

void del_edge_handler(GraphWin& gw) { 
    run_and_display(gw);
}

void del_node_handler(GraphWin& gw) { 
    run_and_display(gw);
}

void init_graph_handler( GraphWin &gw ) { 
    run_and_display(gw);
}

int main(int argc, char *argv[])
{

    // read k
    k = -1;
    if ( argc > 1 ) k = atoi( argv[1] );
    if ( k < 1 ) k = 2;

    GraphWin gw(G,"Approximate Minimum Cycle Basis");

    gw.add_help_text("GW_UNIFORM_UMCB_APPROX");
    gw.display();
    gw.display_help_text("GW_UNIFORM_UMCB_APPROX");

    gw.set_action(A_LEFT | A_DRAG | A_EDGE , NULL);
    gw.win_init(0,200,0);

    // set handlers

    gw.set_init_graph_handler(init_graph_handler);

    gw.set_del_edge_handler(del_edge_handler);
    gw.set_del_node_handler(del_node_handler);
    gw.set_new_edge_handler(new_edge_handler);

    gw.set_directed(false);

    gw.edit();

    return 0;
}

/* ex: set ts=8 sw=4 sts=4 noet: */
