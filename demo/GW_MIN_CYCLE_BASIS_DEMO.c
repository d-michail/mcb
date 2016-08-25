//---------------------------------------------------------------------
// File automatically generated using notangle from MIN_CYCLE_BASIS.lw
// 
// mails and bugs: Dimitris Michail <michail@mpi-sb.mpg.de>
//--------------------------------------------------------------------- 
//
// This program can be freely used in an academic environment
// ONLY for research purposes. Any other use is strictly
// prohibited by the author, without an explicit permission.
//
// Note that this program uses the LEDA library, which is
// NOT free. For more details visit Algorithmic Solutions
// at http://www.algorithmic-solutions.com/
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
// Copyright 2004,2005 - Dimitris Michail



#include <iostream>
#include <LEDA/graphwin.h>
#include <LEDA/graph.h>
#include <LEP/mcb/min_cycle_basis.h>

#if defined(LEDA_NAMESPACE)
using namespace leda;
#endif

graph G;
edge_map<int> Gcost(G);

void run_and_display(GraphWin& gw) { 
        
        bool flush = gw.set_flush(false);

        gw.message("\\bf Computing Minimum Cycle Basis");

        edge_array<int> cost(G);
        node v; edge e;
        forall_edges(e,G) cost[e] = Gcost[e];

        // meet preconditions
        Delete_Loops( G );
        Make_Simple( G );
        G.make_undirected();

        // initialize
        mcb::edge_num enumb( G );
        int c = enumb.num_connected_comp();
        int N = enumb.dim_cycle_space();
        array< d_int_set > mcb;
        array< d_int_set > proof;
        
        int w = mcb::MIN_CYCLE_BASIS_DEPINA( G, cost, mcb, proof, enumb );
        
        forall_edges(e,G) {
                gw.set_color( e, black );
                gw.set_label(e,string("%d (%d)", enumb(e), cost[e] ));
        }

        // set edges of spanning tree as red
        forall_edges( e , G )
                if ( enumb.tree(e) ) 
                        gw.set_color( e, red );

        if ( N > 0 ) { 
                // print cycles
                std::cout << "*** MINIMUM CYCLE BASIS ***" << std::endl;
                std::cout << "Total weight = " << w << std::endl;
                for( int i = 0; i < N; i++ ) { 
                        std::cout << "cycle " << i+1; 
                        std::cout << " : " << mcb[i] << std::endl;
                }
        }

        // compute it also with the hybrid
        /*
        array< d_int_set > mcbh;
        int wh = mcb::MIN_CYCLE_BASIS_DEPINA( G, cost, mcbh, enumb );
        if ( N > 0 ) { 
                // print cycles
                std::cout << "*** CYCLE BASIS by HYBRID ***" << std::endl;
                for( int i = 0; i < N; i++ ) { 
                        std::cout << "cycle " << i+1; 
                        std::cout << " : " << mcbh[i] << std::endl;
                }
        }
        */

        gw.set_flush(flush);
        gw.redraw();
}

void new_node_handler(GraphWin& gw, node) {}

void new_edge_handler(GraphWin& gw, edge e) {
        Gcost[e] = rand_int(0,10);
        gw.set_slider_value(e,Gcost[e]/100.0,1);
        run_and_display(gw);
}

void del_edge_handler(GraphWin& gw) { 
        run_and_display(gw);
}

void del_node_handler(GraphWin& gw) { 
        run_and_display(gw);
}

void init_graph_handler( GraphWin &gw ) { 
        edge e;
        forall_edges(e,G) {
                Gcost[e] = rand_int(1, 10);
                gw.set_slider_value(e,Gcost[e]/100.0,1);
        }
        run_and_display(gw);
}

// rank sliders

void start_cost_slider_handler(GraphWin& gw, edge, double)
{ 
        gw.message("\\bf\\red Change Edge Cost"); 
}

void cost_slider_handler(GraphWin& gw, edge e, double f) 
{ 
        Gcost[e] = int(100*f);
        if( Gcost[e] == 0 ) Gcost[e] = 1;
}

void end_cost_slider_handler(GraphWin& gw, edge, double)
{ 
        run_and_display(gw);
}

int main()
{
        GraphWin gw(G,"Minimum Cycle Basis");

        gw.add_help_text("MIN_CYCLE_BASIS");
        gw.display();
        gw.display_help_text("MIN_CYCLE_BASIS");

        gw.set_action(A_LEFT | A_DRAG | A_EDGE , NULL);
        gw.win_init(0,200,0);

        // set handlers

        gw.set_init_graph_handler(init_graph_handler);

        gw.set_del_edge_handler(del_edge_handler);
        gw.set_del_node_handler(del_node_handler);
        gw.set_new_edge_handler(new_edge_handler);

        gw.set_start_edge_slider_handler(start_cost_slider_handler,1);
        gw.set_edge_slider_handler(cost_slider_handler,1);
        gw.set_end_edge_slider_handler(end_cost_slider_handler,1);
        gw.set_edge_slider_color(blue,1);

        gw.set_directed(false);

        gw.edit();

        return 0;
}

