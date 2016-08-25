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


#line 2936 "MIN_CYCLE_BASIS.lw"
#ifndef SIGNED_GRAPH_H
#define SIGNED_GRAPH_H

#include <iostream>

#ifdef LEDA_GE_V5
#include <LEDA/graph/graph.h>
#include <LEDA/graph/shortest_path.h>
#include <LEDA/graph/node_pq.h>
#include <LEDA/core/array.h>
#include <LEDA/core/d_int_set.h>
#include <LEDA/core/impl/f_heap.h>
#include <LEDA/core/impl/bin_heap.h>
#else
#include <LEDA/graph.h>
#include <LEDA/array.h>
#include <LEDA/shortest_path.h>
#include <LEDA/d_int_set.h>
#include <LEDA/node_pq.h>
#include <LEDA/impl/f_heap.h>
#include <LEDA/impl/bin_heap.h>
#endif
#include <LEP/mcb/edge_num.h>
#include <LEP/mcb/spvecgf2.h>

namespace mcb 
{ 

#if defined(LEDA_NAMESPACE)
using leda::graph;
using leda::node;
using leda::node_array;
using leda::array;
using leda::node_matrix;
using leda::stack;
using leda::list;
using leda::list_item;
using leda::queue;
using leda::node_pq;
using leda::edge;
using leda::edge_array;
using leda::d_int_set;
using leda::error_handler;
#endif


#line 3000 "MIN_CYCLE_BASIS.lw"
template<class W, class prio_impl=leda::bin_heap>
class sgraph 
{
 public:
  
  
#line 3088 "MIN_CYCLE_BASIS.lw"
  sgraph( const graph& G, const edge_array<W>& len, 
	  	  const edge_num& _enumb ) 
	      : weighted(true), g(G), enumb(_enumb) { 
	init_signed_graph();
	sg_len.init( sg , 2*g.number_of_edges(), 0 );

	// initialize edge lengths
	edge e;
	forall_edges( e , g ) { 
	  sg_len[ epos[ e ] ] = len[ e ];
	  sg_len[ eneg[ e ] ] = len[ e ];
	  zero_length_edges = (zero_length_edges==false)?
	                      ( len[ e ] == 0 ):(zero_length_edges);
	}
	if ( sg.number_of_nodes() > 0 ) 
		pq = new node_pq<W,graph,prio_impl>( sg );
	else 
		pq = nil;
	init_all_dijkstra();
  }

  // constructor for unweighted graph
  sgraph( const graph& G,
	  	  const edge_num& _enumb )
	      : weighted(false), g(G), enumb(_enumb) { 
  	init_signed_graph();	      
	//sg_len.init( sg , 2*g.number_of_edges(), 1 );

	// we use again dijkstra's data structures except
	// of the priority queue
	pq = nil;
	init_all_dijkstra();
  }

  ~sgraph() {
  	if ( weighted == true && pq != nil )
		delete pq;
  }

#line 3006 "MIN_CYCLE_BASIS.lw"
  
#line 3233 "MIN_CYCLE_BASIS.lw"
W get_shortest_odd_cycle( const d_int_set& S, d_int_set& C ) { 
    switch_sign( S ); 
    return find_shortest_cycle( S, C ); 
}

W get_shortest_odd_cycle( const spvecgf2& S, spvecgf2& C ) { 

    d_int_set SS; 
    list_item it = S.first();
    while( it != NULL ) { SS.insert( S.inf( it ) ); it = S.succ( it ); }
    
    switch_sign( SS );
    return find_shortest_cycle( S, C );
}


#line 3008 "MIN_CYCLE_BASIS.lw"
  
#line 3179 "MIN_CYCLE_BASIS.lw"
void sign_edge( edge e ) {
	if ( Sold.member( enumb(e) ) ) return;
	node v = g.source(e);
	node u = g.target(e);
	sg.move_edge( epos[ e ] , pos[v] , neg[u] );
	sg.move_edge( eneg[ e ] , neg[v] , pos[u] );
	Sold.insert( enumb(e) );
}

void unsign_edge( edge e ) { 
	if ( Sold.member( enumb(e) ) == false ) return;
	node v = g.source(e);
	node u = g.target(e);
	sg.move_edge( epos[ e ] , pos[v], pos[u] );
	sg.move_edge( eneg[ e ] , neg[v], neg[u] );
	Sold.del( enumb(e) );
} 

// best case O(m)
void set_sign( const d_int_set& S ) {
	edge e; 
	forall_edges( e , g ) 
		if ( S.member( enumb(e) ) ) 
			sign_edge( e ); 
		else 
			unsign_edge( e );
}

// best case O(diff of signs)
void switch_sign( const d_int_set& S ) { 
	d_int_set Sdiff = Sold % S;

	list<int> L;
	Sdiff.get_element_list( L );
	int i;
	edge e;
	forall( i, L ) {
		e = enumb(i);
		if ( Sold.member( i ) )
			unsign_edge( e );
		else
			sign_edge( e );
	}
}

#line 3009 "MIN_CYCLE_BASIS.lw"
  
#line 3613 "MIN_CYCLE_BASIS.lw"
void hide_edge( edge e ) { 
	sg.hide_edge( epos[e] ); 
	sg.hide_edge( eneg[e] ); 
}

void restore_edge( edge e ) { 
	sg.restore_edge( epos[e] ); 
	sg.restore_edge( eneg[e] ); 
}

bool is_hidden( edge e ) { 
	return sg.is_hidden( epos[e] ) || 
		   sg.is_hidden( eneg[e] ); 
}


#line 3011 "MIN_CYCLE_BASIS.lw"
  private:

  
#line 3319 "MIN_CYCLE_BASIS.lw"
  // find the shortest odd cycle
  W find_shortest_cycle( const spvecgf2& S, spvecgf2& C ) {
	d_int_set CC;
	W ret;
#ifndef MCB_LEP_UNDIR_NO_SP_HEURISTIC
  	if ( S.size() <= g.number_of_nodes() )  
		ret = find_shortest_cycle_k ( CC );
	else
#endif
	    ret = find_shortest_cycle_n ( CC );
	C = CC;
	return ret;
  }

  // find the shortest odd cycle
  W find_shortest_cycle( const d_int_set& S, d_int_set& C ) {
#ifndef MCB_LEP_UNDIR_NO_SP_HEURISTIC
  	if ( S.size() <= g.number_of_nodes() )  
		return find_shortest_cycle_k ( C );
	else
#endif
	    return find_shortest_cycle_n ( C );
  }


#line 3014 "MIN_CYCLE_BASIS.lw"
  
#line 3477 "MIN_CYCLE_BASIS.lw"
  /*  Run one iteration of dijkstra. 
   *  Nodes s and t are nodes of the signed graph.
   */
void run_dijkstra( const node& s, const node& t, 
                   const W M, bool M_equals_infinity, 
		   W& D, bool& D_equals_infinity ) {
	node u,w;
	edge e;
	D = W();
	D_equals_infinity = ( s != t )? true : false;
	init_used_dijkstra();
	pred[s] = nil;
	dist[s] = 0;
	touched.push( s );
	if ( D_equals_infinity == false ) return;
	pq->insert( s, 0 );

	// start removing from queue
	while( ! pq->empty() ) {
	  u = pq->del_min();       // get min 

	  if ( u == t ) { // found target
		D_equals_infinity = false;
		D = dist[ u ];
		return;
	  }	

	  // update keys in queue, or add if necessary
	  forall_inout_edges( e , u ) {
		w = sg.opposite( u, e );
		W c = dist[ u ] + sg_len[ e ];

#if !defined(LEDA_CHECKING_OFF)
		if ( c < 0 )
			error_handler(999,"MIN_CYCLE_BASIS:\
			negative distance (overflow)");
#endif
		
		if ( pred[ w ] == nil && w != s ) {  // add
#ifndef MCB_LEP_UNDIR_NO_SP_HEURISTIC
		  // never insert if more than current minimum
		  if ( (! M_equals_infinity) && ( c > M ) ) 
			continue;
#endif
		  pq->insert( w , c );  // first path to w
		  touched.push( w );    // record change
		}
		else if (  c < dist[ w ] ) {  // decrease key
		  pq->decrease_p( w, c );
		}
		else continue;

		dist[ w ] = c;
		pred[ w ] = e;
	  }

	}
  }

#line 3015 "MIN_CYCLE_BASIS.lw"
  
#line 3550 "MIN_CYCLE_BASIS.lw"
// do an unweighted shortest path computation by doing
// a BFS procedure.
// consider the pred[e] array as the visited array
void run_unweighted_sp( const node& s, const node& t, 
                   	int M, bool M_equals_infinity, 
		   	int& D, bool& D_equals_infinity ) 
{ 
	// initialize
	node u,w;
	edge e;
	D = 0;
	D_equals_infinity = ( s != t )? true : false;
	init_used_dijkstra();
	pred[s] = nil;
	dist[s] = 0;
	touched.push( s );
	if ( D_equals_infinity == false ) return;
	Q.append( s );

	// start computation
	while( Q.empty() == false ) {
		u = Q.pop();

		// found target?
		if ( u == t ) { 
			D = dist[ u ];
			D_equals_infinity = false;
			return;
		}

		// if we reach upper bound, exit immediately
		if ( M_equals_infinity == false && dist[u] > M )
			return;
	
		// visit neighbors
		forall_inout_edges( e, u ) {
			w = g.opposite( e, u );
			if ( w != s && pred[ w ] == nil ) { 
				pred[ w ] = e;
				dist[ w ] = dist[ u ] + 1;
				touched.push( w );
				Q.append( w );
			}
		}
	}
}


#line 3016 "MIN_CYCLE_BASIS.lw"
  

#line 3379 "MIN_CYCLE_BASIS.lw"
  // Find the currently shorter odd cycle, by n dijkstra computations. 
  W find_shortest_cycle_n( d_int_set& C )
    {
	  W M = W(0), D = W(0);    // current minimum
	  bool M_equals_infinity = true, D_equals_infinity = true;
	
	  // run n times dijkstra's algorithm
	  node v;
	  forall_nodes( v , g ) { 

		if ( weighted ) 
			run_dijkstra( pos[v], neg[v], 
				      M, M_equals_infinity, 
				      D, D_equals_infinity );
		else
			run_unweighted_sp( pos[v], neg[v], 
					   M, M_equals_infinity, 
					   D, D_equals_infinity );

		// update global min
		if ( ! D_equals_infinity ) {
		  if ( M_equals_infinity ) { 
			if ( check_valid( neg[v], pred, C ) ) { 
			  M = D;
			  M_equals_infinity = false;
			}
		  }
		  else {
			if ( D < M ) { 
			  if ( check_valid( neg[v], pred, C ) ) { 
				M = D;
			  }
			}
		  }
		}
	  }
	  return M;
    }


#line 3017 "MIN_CYCLE_BASIS.lw"
  
#line 3662 "MIN_CYCLE_BASIS.lw"
/* Find a shortest odd cycle by doing only |S| dijkstra runs.
 * Usefull if k < n, which seems to be the case.
 */
W find_shortest_cycle_k( d_int_set& C ) { 
	// initialize
	W M = W(0), D = W(0);
	bool M_equals_infinity = true, D_equals_infinity = true;
	
	// get edge list and hide edges
	edge e;
	list<int> L;
	Sold.get_element_list( L );
	int i;
	forall(i,L) hide_edge( enumb(i) ); 
	
	// now do the computations
	forall(i,L) { 
		e = enumb(i);

		// NOTE:
		// M is an upper bound on the previous cycles found
		// In this case M is not exactly tight, since dijkstra 
		// is finding a path which is missing an edge in order 
		// to be a cycle.
		// I do not see however a clean way to make it more tight.

		if ( weighted )
			run_dijkstra( pos[ g.source(e) ], 
					  pos[ g.target(e) ], 
					  M, M_equals_infinity, 
					  D, D_equals_infinity );
		else 
			run_unweighted_sp( pos[ g.source(e) ], pos[ g.target(e) ],
					   M, M_equals_infinity,
					   D, D_equals_infinity );
					  
		restore_edge( e );
	
		if ( D_equals_infinity == true ) 
			continue;
		if ( M_equals_infinity == false && 
			 M <= D + ( weighted?sg_len[ epos[e] ]: 1 ) ) 
			continue;
		if ( check_valid( pos[ g.target(e) ], pred, C ) == false ) 
			continue;
		C.insert( i );
		M = D + ( weighted? sg_len[ epos[e] ] : 1 );
		M_equals_infinity = false;
	}
	return M;
}


#line 3019 "MIN_CYCLE_BASIS.lw"
  
#line 3437 "MIN_CYCLE_BASIS.lw"
  /*  Check if a path found in the signed graph, is valid for the original one. 
   *  A path is valid for the original graph, if the resulting cycle is simple.
   *  \param u The node in the signed graph, to start from (target).
   *  \param V If valid, this contains the path, otherwise this is undefined.
   *  \param pred The predecessor array from dijkstra.
   */ 
  bool check_valid(node u, node_array<edge>& pred, d_int_set& V) 
    {
	  Stemp.clear();
	  node t = u;
	  while (pred[t] != nil ) {
		if ( Stemp.member( enumb( remap[ pred[t] ] ) ) ) { 
		  return false;
		} 
		else Stemp.insert( enumb( remap[ pred[t] ] ) );
		t = sg.opposite( pred[ t ], t );
	  }
	  if ( Stemp.size() > 0 ) { 
	  	V = Stemp;
		return true;
	  }
	  return false;
    }


#line 3021 "MIN_CYCLE_BASIS.lw"
  
#line 3136 "MIN_CYCLE_BASIS.lw"
// init signed graph without edge lengths
void init_signed_graph() { 
	node v; edge e;

	// initialize mapping
	pos.init( g ); neg.init( g ); epos.init( g ); eneg.init( g );
	forall_nodes( v , g ) { 
		pos[ v ] = sg.new_node(); 
		neg[ v ] = sg.new_node(); 
	}
	remap.init( sg , 2*g.number_of_edges(), nil );

	// initialize sign to the empty sign
	zero_length_edges = false;
	forall_edges( e , g ) { 
	  edge e1 = sg.new_edge( pos[ g.source(e) ], pos [ g.target(e) ] ) ;
	  epos[ e ] = e1;
	  edge e2 = sg.new_edge( neg[ g.source(e) ], neg [ g.target(e) ] );
	  eneg[ e ] = e2;
	  remap[ e1 ] = e;
	  remap[ e2 ] = e;
	  zero_length_edges = false;
	}
}

#line 3022 "MIN_CYCLE_BASIS.lw"
  
#line 3057 "MIN_CYCLE_BASIS.lw"
  // Mapping related
  bool weighted;  // is the graph weighted or not?
  const graph& g; // the original graph
  const edge_num& enumb;
  graph sg; 
  edge_array<W> sg_len; 

  // positive nodes (indexed by g, returns nodes by sg)
  node_array<node> pos; 
  // negative nodes (indexed by g, returns nodes by sg)
  node_array<node> neg; 
  // two edges for each edge (indexed by g, returns edges by sg)
  edge_array<edge> epos,eneg;  
  // reverse edge mapping (indexed by sg, returns edges by g)
  edge_array<edge> remap; 
  // do zero length edges exist?
  bool zero_length_edges; 
  // sign, indexed by g
  d_int_set Sold,Stemp;

#line 3023 "MIN_CYCLE_BASIS.lw"
  
#line 3274 "MIN_CYCLE_BASIS.lw"
stack<node> touched;               // keep track of changes
node_array<W> dist;                // distance
node_array<edge> pred;             // predecessor
node_pq<W,graph,prio_impl> *pq;    // priority queue
queue<node> Q;			   // priority queue for unweighted case (BFS)

// init all dijkstra's data
void init_all_dijkstra() { 
	dist.init( sg, 0 ); 
	pred.init( sg, nil ); 
	touched.clear(); 
	if ( pq != nil ) 
		pq->clear();
	Q.clear();
}

// init only used dijkstra's data
void init_used_dijkstra() { 
	while( ! touched.empty() ) { 
		node w = touched.pop(); 
		dist[w] = 0; 
		pred[ w ] = nil;  
	}	
	if ( pq != nil ) 
		pq->clear();
	Q.clear();
}

#line 3024 "MIN_CYCLE_BASIS.lw"
};


#line 2983 "MIN_CYCLE_BASIS.lw"
} // namespace mcb end

#endif

#line 6217 "MIN_CYCLE_BASIS.lw"
/* ex: set ts=8 sw=4 sts=4 noet: */


