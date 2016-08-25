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


#ifndef DIR_MIN_CYCLE_BASIS_H
#define DIR_MIN_CYCLE_BASIS_H

#ifdef LEDA_GE_V5
#include <LEDA/graph/graph.h>
#include <LEDA/graph/edge_array.h>
#include <LEDA/core/array.h>
#include <LEDA/core/list.h>
#include <LEDA/core/p_queue.h>
#include <LEDA/core/random_source.h>
#include <LEDA/core/sortseq.h>
#include <LEDA/core/tuple.h>
#include <LEDA/numbers/integer.h>
#else
#include <LEDA/graph.h>
#include <LEDA/array.h>
#include <LEDA/edge_array.h>
#include <LEDA/list.h>
#include <LEDA/integer.h>
#include <LEDA/p_queue.h>
#include <LEDA/random_source.h>
#include <LEDA/sortseq.h>
#include <LEDA/tuple.h>
#endif

#include <LEP/mcb/edge_num.h>
#include <LEP/mcb/fp.h>
#include <LEP/mcb/arithm.h>

namespace mcb
{

#if defined(LEDA_NAMESPACE)
    using leda::graph;
    using leda::edge;
    using leda::edge_array;
    using leda::node;
    using leda::node_array;
    using leda::array;
    using leda::list;
    using leda::node_list;
    using leda::integer;
    using leda::random_source;
    using leda::p_queue;
    using leda::pq_item;
    using leda::sortseq;
    using leda::seq_item;
    using leda::three_tuple;
    using leda::four_tuple;
#endif

    /*{\Mtext

       \newpage

       {\bf \large Directed Minimum Cycle Basis Algorithms}\medskip

       \settowidth{\typewidth}{|int|}
       \addtolength{\typewidth}{\colsep}
       \computewidths

       Given an directed graph $G(V,E)$ and a non-negative length function on the edges
       $w: E \mapsto \mathcal{R}_{\ge 0}$, a minimum cycle basis is a set of cycles
       which can generate the cycle space and at the same time has minimum total length.

       Each cycle of the graph is assumed to be a vector in $Q^m$ indexed on the edge set, and
       operations between cycles is performed in $Q$. The length of a cycle basis is 
       the sum of the length of its cycles and the length of a cycle is the sum of the 
       length of its edges.

       Most functions of this section are templates functions. The template parameter |W| denoting the 
       type of the edge weights can be instantiated with any number type. Attention must be paid in order 
       to avoid overflow of values.

       The solution of a minimum cycle basis problem can be in the following three forms.
       \begin{enumerate}
       \item A pair |(mcb, proof)| where both are arrays of |spvecfp|. A |spvecfp| is a 
       wrapper around Leda's list datatype. Each element in this list, is a 
       |two_tuple<long,integer>|, where the first argument is its index $(1 \dots m)$ and
       the second is in case of |mcb| a value of  ${\pm}1$ where positive is an arbitrary 
       direction of traversing the cycle and in case of |proof| a value in $Z_p$ for 
       some prime number $p$. 
       Each index represents an edge. {\em NOTE: index $i$ represents edge $i-1$.} The number
       of an edge can be found by the edge numbering, |edge_num|.
       \item A solution |mcb| which is an array of list of edges, |list<edge>|.
       Each element of this array represents a cycle of the minimum cycle basis.
       \item A solution |mcb| which is an array of arrays of mcb::etype (currently short ints), 
       |array<mcb::etype>|.
       Each element of this array represents a cycle of the minimum cycle basis.

       Each entry $i$ of the array is $0$ or ${\pm}1$, based on whether the edge $e$ with 
       number $i$ ( |enumb(e) = i| ) belongs to the cycle and if yes in which direction
       compared with an arbitrary direction of traversing the cycle.
       \end{enumerate}

       The whole package is protected using a namespace called "mcb", and therefore using 
       anything requires {\em mcb::xxx} or the {\em using namespace mcb} directive.

       } */


    // from sparse vector to list of edges
    void spvecfp_to_list_edges( const graph & g, const edge_num & enumb,
				const spvecfp & in, list < edge > &out );

    // from sparse vector to array of integers
    void spvecfp_to_array_ints( const graph & g, const edge_num & enumb,
				const ptype & p, const spvecfp & in,
				array < etype > &out );

    // verify that a sparse vector represents a cycle
    bool DMCB_verify_cycle( const graph & g,
			    const edge_num & enumb, const spvecfp & cycle );

    // check that all cycles of an array are indeed cycles
    // precondition, cycles array is C style [0..size-1]
    bool DMCB_verify_cycles( const graph & g,
			     const edge_num & enumb,
			     const array < spvecfp > &cycles );

    // check that set of cycles are really a Basis
    bool DMCB_verify_basis( const graph & g,
			    const edge_num & enumb,
			    const array < spvecfp > &B,
			    const array < spvecfp > &X );



    // defines for SSitem
#define mcb_revi() first()
#define mcb_dist() second()
#define mcb_num_edges() third()
#define mcb_pred() fourth()

    // defines for preditem
#define mcb_edge() first()
#define mcb_other_node_index() second()
#define mcb_other_node() third()


    // T = weight
    // PT = type of prime p
      template < class T, class PT > class dirsp
    {

	typedef three_tuple < edge, PT, node > preditem;
	typedef four_tuple < pq_item, T, indextype, preditem > SSitem;
	typedef two_tuple < node, PT > pnode;

      public:


	// constructor
	  dirsp( const graph & g, const edge_array < T > &length,
		 const PT & pin,
		 const edge_num & en ):n( g.number_of_nodes(  ) ),
	    m( g.number_of_edges(  ) ), p( pin ), G( g ), SS( mcb::compare ),
	    len( length ), enumb( en ), cycle( m ), nodeid( g ), marked( g,
									 0 ),
	    Xe( g )
	{
	    // give a numbering to the nodes
	    indextype i = 0;
	    node v;
	      forall_nodes( v, g )
	    {
		nodeid[v] = i;
		i++;
	    }
	}

	// reinitialize with different p
	// used for execution on same graph but different p
	// in order to reduce error probability
	void reinit( const PT & pin )
	{
	    p = pin;
	}

	// destructor
	~dirsp(  ) {
	}


	// compute shortest cycle
	spvecfp get_shortest_cycle( const spvecfp & X, T & weight )
	{

	    // create X, on edge index
	    fill_X( X );

	    // run shortest paths
	    weight = run_shortest_paths(  );

	    // construct sparse vector from cycle
	    spvecfp ret( m, p );

	    for ( indextype i = 0; i < m; i++ ) {
		if ( cycle[i] != 0 ) {
		    ret.append( i + 1, cycle[i] );
		}
	    }

	    return ret;
	}

      private:

	// get X on edge index, by spvecfp
	void fill_X( const spvecfp & X )
	{
	    edge e;

	    forall_edges( e, G )
		Xe[e] = 0;

	    list_item li = X.first(  );

	    while ( li != nil ) {
		// note: spvecfp is indexed on 1..m
		//       but enumb on 0..m-1
		Xe[enumb( X.index( li ) - 1 )] = X.inf( li );
		li = X.succ( li );
	    }
	}


	// record a cycle, by traversing pred array
	void record_cycle( const PT & t, array < etype > &cyc )
	{
	    cyc.init( 0 );

	    PT cur = t;
	    seq_item sicur = SS.lookup( cur );
	    node u;
	    edge e;

	    while ( SS[sicur].mcb_pred(  ).mcb_edge(  ) != nil ) {

		u = SS[sicur].mcb_pred(  ).mcb_other_node(  );
		e = SS[sicur].mcb_pred(  ).mcb_edge(  );
		if ( u == G.target( e ) ) {	// give + direction
		    cyc[enumb( e )] = 1;
		} else {	// give - direction
		    cyc[enumb( e )] = -1;
		}

		cur = SS[sicur].mcb_pred(  ).mcb_other_node_index(  );
		sicur = SS.lookup( cur );
	    }
	}


	// get the index of a node in the reverse index of the PQ
	inline PT get_revi_index( const node & v, const PT & level )
	{
	    return level * n + nodeid[v];
	}


	// init used data structures
	void init_used_dijkstra(  )
	{
	    // init marked
	    while ( marked_touched.empty(  ) == false )
		marked[marked_touched.pop(  )] = 0;

	    // init all nodes reached by SP
	    SS.clear(  );

	    // clear priority queue
	    PQ.clear(  );
	}


	// execute the shortest paths
	T run_shortest_paths(  )
	{

	    T minall;
	    indextype minall_num_edges;
	    bool is_minall_inf = true;


	    node v;

	    forall_nodes( v, G ) {	// do n shortest paths

		T min;
		indextype min_num_edges;
		bool is_min_inf = true;

		// add source vertex to PQ
		PT vi = get_revi_index( v, 0 );

		SS.insert( vi,
			   SSitem( PQ.insert( T( 0 ), pnode( v, 0 ) ),
				   T( 0 ), 0, preditem( nil, 0, nil )
			    )
		     );


		// do shortest path
		pq_item pqi;
		node u, w;	// nodes
		PT ul, wl;	// levels
		PT uindex, windex;	// index on arrays
		seq_item siu, siw;
		edge e;

		while ( PQ.empty(  ) == false ) {
		    // extract min
		    pqi = PQ.find_min(  );
		    u = PQ[pqi].first(  );	// get vertex
		    ul = PQ[pqi].second(  );	// get level
		    PQ.del_item( pqi );

		    // find index of u in level graph
		    uindex = get_revi_index( u, ul );
		    siu = SS.lookup( uindex );

		    // check if we are done
		    if ( u == v && ul != 0 ) {
			min = SS[siu].mcb_dist(  );
			min_num_edges = SS[siu].mcb_num_edges(  );
			is_min_inf = false;
			break;
		    }

		    // check if vertex is too far
		    if ( marked[u] >= 2 )
			continue;

		    // increment marked
		    marked[u] = marked[u] + 1;
		    if ( marked[u] == 1 )
			marked_touched.append( u );



		    // find neighbors
		    forall_inout_edges( e, u ) {

			w = G.opposite( e, u );

			// find level of w
			if ( w == G.target( e ) ) {	// e = u -> w
			    wl = ( ul + Xe[e] ) % p;
			} else {	// e = w -> u
			    wl = ( ul - Xe[e] ) % p;
			}
			while ( wl < 0 )
			    wl += p;	// [-i]_p = [p-i]_p


			// find index in level graph
			windex = get_revi_index( w, wl );
			siw = SS.lookup( windex );

			// now update if necessary
			T c = SS[siu].mcb_dist(  ) + len[e];

			if ( siw == nil ) {
			    // don't insert if more than current minimum
			    if ( is_minall_inf == false && c > minall )
				continue;
			    // insert ( for now, with some info undetermined )
			    siw = SS.insert( windex,
					     SSitem( PQ.
						     insert( c,
							     pnode( w, wl ) ),
						     c, 0, preditem( nil, 0,
								     nil ) ) );
			} else if ( c < SS[siw].mcb_dist(  ) ) {
			    PQ.decrease_p( SS[siw].mcb_revi(  ), c );
			} else
			    continue;

			// record changes
			SS[siw].mcb_dist(  ) = c;
			SS[siw].mcb_num_edges(  ) =
			    SS[siu].mcb_num_edges(  ) + 1;
			SS[siw].mcb_pred(  ).mcb_edge(  ) = e;
			SS[siw].mcb_pred(  ).mcb_other_node_index(  ) =
			    uindex;
			SS[siw].mcb_pred(  ).mcb_other_node(  ) = u;
		    }

		}



		// possible record path
		if ( is_min_inf == false ) {
		    if ( ( is_minall_inf == true ) ||
			 ( min < minall ) ||
			 ( ( min == minall )
			   && ( min_num_edges < minall_num_edges ) )
			 ) {
			// record new path as better either if it has less weight
			// or if the weight is equal and it has less number of edges

			// record path to a cycle
			record_cycle( get_revi_index( u, ul ), cycle );
			minall = min;
			minall_num_edges = min_num_edges;
			is_minall_inf = false;
		    }
		}
		// init touched data
		init_used_dijkstra(  );
	    }

	    // now return path found
	    if ( is_minall_inf == true )
		leda::error_handler( 999,
				     "MIN_CYCLE_BASIS: no cycle found :(" );

	    return minall;
	}



	// data structures
	int n, m;
	PT p;
	const graph & G;
	const edge_array < T > &len;	// edge lengths
	const edge_num & enumb;	// edge numbering

	p_queue < T, pnode > PQ;	// priority queue

	// sortseq keys on node id of a node in levelg
	// SSitem
	// contains first()  = reverse index on priority queue, 
	//                     node(in levelg) -> pq_item
	//          second() = distance of shortest path in 
	//                     levelg to this node
	//          third()  = number of edges of shortest path in 
	//                     levelg to this node
	//          fourth() = predecessor as three_tuple in levelg 
	//                     to this node
	//                     preditem
	//                     contains first()  = edge e of g
	//                              second() = node id of other 
	//                                         end in levelg
	//                              third()  = node in g on other end
	sortseq < PT, SSitem > SS;

	array < etype > cycle;	// incident vector of cycles, indexed on 
	// numbering of edges (enumb(e))
	node_array < PT > nodeid;	// nodes' ids   of g
	node_array < unsigned short >marked;	// marked nodes of g 
	node_list marked_touched;	// touched marked nodes 

	edge_array < PT > Xe;	// help set to construct the cycle
    };

    // now undefine, all defined stuff for SSitem and preditem
#undef mcb_revi
#undef mcb_dist
#undef mcb_num_edges
#undef mcb_pred
#undef mcb_edge
#undef mcb_other_node_index
#undef mcb_other_node

    // W = weight type
    // ptype = type of prime p
    template < class W >
	W DIR_MIN_CYCLE_BASIS( const graph & g,
			       const edge_array < W > &len,
			       array < spvecfp > &mcb,
			       array < spvecfp > &proof,
			       const edge_num & enumb, double error = 0.375 )
	/*{\Mfunc $g$ is a directed graph and $len$ is a positive length function 
	   on the edges of $g$. The function computes a directed Minimum Cycle Basis $B$ of $g$, that 
	   is a cycle basis of $g$ with the smallest length (sum of lengths of cycles). The 
	   basis is returned as an array of sparse vectors |spvecfp|, called $mcb$. \\
	   Every edge is represented by some integer, by a fixed and arbitrary numbering. This
	   numbering is represented by $enumb$. \\
	   The function also returns a certificate of optimality of 
	   the minimum cycle basis. More precisely a set of linearly independent vectors for 
	   which cycle $i$ is the shortest cycle in $g$ with non-zero intersection with the 
	   proof vector $i$. \\
	   Since the algorithm is a randomized Monte-Carlo algorithm, the error argument 
	   which should be less that 1 represents the acceptable error probability that the
	   returns cycle basis is not a minimum cycle basis.
	   The function returns the weight of the Minimum Cycle Basis or is undefined
	   if there were any errors. \\
	   The running time is $O( m^3 )$ where $m$ are the number of edges of $g$. \\
	   \precond $g$ is simple and loopfree \\
	   \precond $len$ is positive \\
	   \precond $enumb$ is already initialized with $g$ \\
	   \precond $error$ is positive and less than one \\
	   } */
    {

#if ! defined(LEDA_CHECKING_OFF)
	if ( Is_Simple( g ) == false )
	    error_handler( 999,
			   "MIN_CYCLE_BASIS: illegal graph (non-simple?)" );
	if ( Is_Loopfree( g ) == false )
	    error_handler( 999,
			   "MIN_CYCLE_BASIS: illegal graph (has loops?)" );
	if ( error <= 0 || error >= 1 )
	    error_handler( 999,
			   "MIN_CYCLE_BASIS: error probability is out of range" );

	edge e1;

	forall_edges( e1, g ) {
	    if ( len[e1] <= 0 )
		error_handler( 999,
			       "MIN_CYCLE_BASIS: illegal edge (non-positive weight)" );
	}
#endif

	int d = enumb.dim_cycle_space(  );

	// nothing to compute
	if ( d <= 0 )
	    return W( 0 );

	mcb.resize( d );
	proof.resize( d );

	int m = g.number_of_edges(  );
	int n = g.number_of_nodes(  );


	// decide how many times to execute the algorithm ( perror <= 3/8 = 0.375 )
	int times = ( int ) ceil( log( error ) / log( 0.375 ) );

#if  defined(LEP_DEBUG_OUTPUT)
	std::cout << "Executing " << times;
	std::cout << " number of times to achieve error probability ";
	std::cout << error << std::endl;
#endif

	// create X and B matrices
	array < spvecfp > X( d );
	array < spvecfp > B( d );
	W min_so_far;
	bool min_so_far_inf = true;

	// initialize shortest paths
	// for now use any prime number = 7
	dirsp < W, ptype > SP( g, len, 7, enumb );

	// loop necessary times, for error probability to be correct
	while ( times-- > 0 ) {

	    // pick random prime
	    ptype p;

	    {
		int logd = log( integer( d + 1 ) );
		int loglogd = log( integer( logd + 1 ) );
		int randbits = 7 + 2 * logd + loglogd;
		int failsafe = 50 * randbits;
		int count = 0;

		while ( true ) {
		    // loop failsafe, increase random bits
		    if ( count++ > failsafe ) {
			randbits++;
			failsafe += 50;
			count = 0;
		    }
		    // get random number
		    p = ptype::random( randbits );
		    p += d * d;
		    //std::cout << "testing p = " << p << std::endl;

		    // if is > 1 and prime break
		    if ( p > 1 && primes < ptype >::is_prime( p ) )
			break;
		}
	    }

#if  defined(LEP_DEBUG_OUTPUT)
	    std::cout << "executing with prime p = " << p << std::endl;
#endif

	    // initialize shortest path class for this particular prime
	    SP.reinit( p );

	    // initialize X_i's and $B_i$'s
	    // assume that $p$ fits in ptype
	    // and $d$ in indextype
	    indextype i, j;

	    for ( i = 0; i < d; i++ ) {
		X[i] = spvecfp( m, p );
		X[i] = i + 1;
		B[i] = spvecfp( m, p );
		B[i] = 0;
	    }


	    // now execute main loop
	    spvecfp tmp = spvecfp( m, p );
	    ptype tmpi;
	    W min = W( 0 );

	    for ( i = 0; i < d; i++ ) {

		// compute B_i
		W mini;

		B[i] = SP.get_shortest_cycle( X[i], mini );
		min += mini;

		// precompute part
		// NOTE: we do not precompute inverses, since we don't want
		//       to have a dependency on the maximum size of an 
		//       array that will store these values
		//       p is O(d^2 logd) and thus O(logd) to compute inverse
		//       at most d times, thus O(d logd) = O(m logm) in total
		tmpi = X[i] * B[i];
		while ( tmpi < 0 )
		    tmpi += p;	// make [-i]_p = [p-i]_p
		while ( tmpi >= p )
		    tmpi -= p;	// make [i+p]_p = [i]_p
		tmp = X[i] * fp < ptype >::get_mult_inverse( tmpi, p );

		// update sets X_j, j > i
		for ( j = i + 1; j < d; j++ )
		    X[j] -= tmp * ( B[i] * X[j] );
	    }


	    // if found better, update
	    if ( ( min_so_far_inf == true ) ||
		 ( min_so_far_inf == false && min < min_so_far ) ) {
#if  defined(LEP_DEBUG_OUTPUT)
		if ( min_so_far_inf == false )
		    std::
			cout << "found better solution with weight " << min <<
			std::endl;
#endif
		mcb = B;
		proof = X;
		min_so_far_inf = false;
		min_so_far = min;
	    }

	}


	return min_so_far;
    }				// end of DIR_MIN_CYCLE_BASIS


    template < class W >
	W DIR_MIN_CYCLE_BASIS( const graph & g,
			       const edge_array < W > &len,
			       array < array < etype > >&mcb,
			       const edge_num & enumb, double error = 0.375 )
	/*{\Mfunc As above but the
	   basis is returned as an array of arrays of ints. \\
	   Each such array is indexed on $1 \dots m$ and its entry can be $0$ or $\pm 1$. Which 
	   edge corresponds to which index in this array can be found by the edge numbering, |enumb|. \\
	   } */
    {
	array < spvecfp > mcb_tmp;
	array < spvecfp > proof_tmp;

	int d = enumb.dim_cycle_space(  );

	// run the general version
	W min = DIR_MIN_CYCLE_BASIS < W > ( g, len, mcb_tmp,
					    proof_tmp, enumb, error );

	// get p used
	ptype p = proof_tmp[0].pvalue(  );

	// transform
	mcb.resize( d );
	for ( int i = 0; i < d; i++ )
	    spvecfp_to_array_ints( g, enumb, p, mcb_tmp[i], mcb[i] );

	return min;
    }


    template < class W >
	W DIR_MIN_CYCLE_BASIS( const graph & g,
			       const edge_array < W > &len,
			       array < list < edge > >&mcb,
			       double error = 0.375 )
	/*{\Mfunc As above but the basis is returned as an array of list of edges. \\
	   } */
    {
	edge_num enumb( g );

	array < spvecfp > mcb_tmp;
	array < spvecfp > proof_tmp;

	int d = enumb.dim_cycle_space(  );

	// run the general version
	W min = DIR_MIN_CYCLE_BASIS < W > ( g, len, mcb_tmp,
					    proof_tmp, enumb, error );

	// transform
	mcb.resize( d );
	for ( int i = 0; i < d; i++ )
	    spvecfp_to_list_edges( g, enumb, mcb_tmp[i], mcb[i] );

	return min;
    }


    // from array of integers to sparse vectors
    void array_ints_to_spvecfp( const graph & g, const edge_num & enumb,
				const ptype & p, const array < etype > &in,
				spvecfp & out )
    {
	int m = g.number_of_edges(  );

	out.reset( m, p );

	indextype i;

	for ( i = 0; i < m; i++ ) {
	    if ( in[i] != 0 ) {
		out.append( i + 1, ptype( in[i] ) );
	    }
	}
    }




}				// end of mcb namespace

#endif // DIR_MIN_CYCLE_BASIS_H

/* ex: set ts=8 sw=4 sts=4 noet: */
