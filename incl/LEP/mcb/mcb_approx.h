
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


/*! \file mcb_approx.h
 *  \brief Algorithms for approximate MCB.
 *
 *  \sa mcb::edge_num mcb::spvecgf2 mcb::spvecfp
 */

#ifndef MCB_APPROX_H
#define MCB_APPROX_H

#include <LEP/mcb/config.h>
#include <LEP/mcb/min_cycle_basis.h>
#include <LEP/mcb/spanner.h>
#include <LEP/mcb/ushortpath.h>
#include <LEP/mcb/dir_min_cycle_basis.h>

#ifdef LEDA_GE_V5
#include <LEDA/graph/edge_map.h>
#include <LEDA/graph/node_map.h>
#include <LEDA/core/impl/f_heap.h>
#include <LEDA/core/impl/bin_heap.h>
#else
#include <LEDA/edge_map.h>
#include <LEDA/node_map.h>
#include <LEDA/impl/f_heap.h>
#include <LEDA/impl/bin_heap.h>
#endif

// start our namespace
namespace mcb 
{ 

#if defined(LEDA_NAMESPACE)
using leda::node;
using leda::node_map;
using leda::edge;
using leda::edge_array;
using leda::edge_map;
#endif

/*! \name Undirected Approximate Minimum Cycle Basis
 */
//@{
/*! \brief Compute an undirected approximate MCB of a weighted graph. 
 * 
 *  The function computes an \f$(2k-1)\f$-approximate Minimum Cycle
 *  Basis \f$B\f$ of a graph \f$g\f$.
 *  The basis is returned as an array of sparse vectors, spvecgf2, called mcb.<br>
 *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
 *  numbering is represented by enumb.<br>  
 *
 *  The function returns the weight of the approximate Minimum Cycle Basis or is undefined
 *  if there were any errors.<br> Even if the graph is directed this function computes an 
 *  approximate MCB of the underlying undirected graph.<br>
 *
 *  The running time is \f$O( m n^{1+1/k} + \min( m^3 + mn^2 \log n, n^{3+3/k}) )\f$ where \f$n\f$ are the number of nodes 
 *  of \f$g\f$, \f$m\f$ the number of edges and \f$k\f$ is an integer \f$\ge 1\f$.
 *
 *  \param g An graph.
 *  \param len A leda::edge_array for the edge lengths.
 *  \param k How much to approximate?
 *  \param mcb A leda::array of spvecgf2 to return the MCB.
 *  \param enumb An edge numbering.
 *  \return The length of the approximate MCB or undefined if some error occured.
 *  \pre g is loopfree.
 *  \pre len is non-negative
 *  \pre k must be an integer greater than zero
 *  \remark Care must be taken when the template parameter is instantiated with a data type
 *          which has rounding errors.
 *  \ingroup approxmcb
 */
template<class W>
W UMCB_APPROX( const graph& g,
	       const edge_array<W>& len,
	       const int k,
	       array< mcb::spvecgf2 >& mcb,
	       const mcb::edge_num& enumb
) 
{ 
    // check preconditions
#if ! defined(LEDA_CHECKING_OFF)
    if ( Is_Loopfree( g ) == false )
	error_handler(999,"UMCB_APPROX: illegal graph (loops?)");
    if ( k <= 0 )
	error_handler(999,"UMCB_APPROX: illegal value of k, non-positive?");
    edge e1;
    forall_edges( e1 , g ) { 
	if ( len[e1] < 0 ) 
	    error_handler(999,"UMCB_APPROX: illegal edge (negative weight?)");
    }
#endif

    W length = W();
    int N = enumb.dim_cycle_space();
    mcb.resize( N ); // set size of destination array

#ifdef LEP_STATS
    float Tcycles = 0.0, Tsubgraph = 0.0, Ttemp;
    leda::used_time( Ttemp );
#endif
    
    // construct spanner and mappings
    graph spanner;
    node_map<node> node_g_to_spanner;
    node_map<node> node_spanner_to_g;
    edge_map<edge> edge_g_to_spanner;
    edge_map<edge> edge_spanner_to_g;

    mcb::SPANNER( g, len, k, spanner, 
	    node_g_to_spanner, node_spanner_to_g,
	    edge_g_to_spanner, edge_spanner_to_g,
	    enumb);

    // construct spanner edge lengths
    edge e, f;
    edge_array<W> spanner_len ( spanner );
    forall_edges( e, spanner ) { 
	spanner_len[ e ] = len[ edge_spanner_to_g[ e ] ];
    }

    // now compute partial MCB cycles based on spanner
    ushortestpaths<W> usp( spanner, spanner_len );
    int i = 0;
    forall_edges( e, g ) { 
	if ( edge_g_to_spanner[e] == nil ) { 
	    mcb[i] = mcb::spvecgf2();
		
	    // compute shortest path on spanner
	    node spanner_s = node_g_to_spanner[ g.source(e) ];
	    node spanner_t = node_g_to_spanner[ g.target(e) ];
	    usp.compute_shortest_path( spanner_s, spanner_t  );
	    
#if ! defined(LEDA_CHECKING_OFF)
	    assert( usp.is_reachable( spanner_t ) && usp.pred( spanner_s ) == nil );
#endif

	    // form cycle
	    W cycle_len = W();
	    node spanner_w = spanner_t;
	    while( usp.pred( spanner_w ) != nil ) { 
		f = usp.pred( spanner_w );
		mcb[i].insert( enumb( edge_spanner_to_g[ f ] ) );
		cycle_len += len[ edge_spanner_to_g[ f ] ];
		spanner_w = spanner.opposite( f, spanner_w );
	    }
	    mcb[i].insert( enumb( e ) );
	    cycle_len += len[ e ];
	    mcb[i].sort(); // fix correct ordering

	    // now update global cycles length
#if ! defined(LEDA_CHECKING_OFF)
	    if ( cycle_len < 0 ) 
		error_handler(999,"UMCB_APPROX: computed cycle with negative length!");
#endif
	    length += cycle_len;
	    
	    i++;
	}
    }

#ifdef LEP_DEBUG_OUTPUT
    std::cout << "Spanner has " << spanner.number_of_edges() << " edges..." << std::endl;
    std::cout << "Computed " << i << " cycles fast..." << std::endl;
#endif

#ifdef LEP_STATS
    Tcycles += leda::used_time( Ttemp );
#endif

    // now compute the MCB of the spanner
    edge_num spanner_enumb( spanner );
    array< spvecgf2 > spanner_mcb, spanner_proof;

#ifdef LEP_DEBUG_OUTPUT
    std::cout << "Computing " << spanner_enumb.dim_cycle_space() << " cycles"; 
    std::cout << " by the MCB of spanner..." << std::endl;
#endif

    length += MIN_CYCLE_BASIS_DEPINA<W,mcb::spvecgf2>( spanner, spanner_len,
	    spanner_mcb, spanner_proof, spanner_enumb );
    

    // now translate to the original graph
    int extracycles = spanner_enumb.dim_cycle_space();
    for( i = 0; i < extracycles; ++i ) 
    {
	mcb[ N - extracycles + i ] = spvecgf2();
	
	int j = 0;
	forall( j, spanner_mcb[ i ] ) 
	{ 
	    // translate to numbering of original graph 
	    // and add to cycle
	    e = edge_spanner_to_g[ spanner_enumb(j) ];
	    mcb[ N - extracycles + i ].insert( enumb( e ) );
	}
	// fix correct ordering
	mcb[ N - extracycles + i ].sort();
    }

#ifdef LEP_STATS
    Tsubgraph += leda::used_time( Ttemp );
    std::cout << "LEP_STATS: spanner cycles time: " << Tcycles << std::endl;
    std::cout << "LEP_STATS: subgraph MCB time  : " << Tsubgraph << std::endl;
#endif

    return length;
}


/*! \brief Compute an undirected approximate MCB of a graph. 
 * 
 *  The function computes an \f$(2k-1)\f$-approximate Minimum Cycle
 *  Basis \f$B\f$ of a graph \f$g\f$.
 *  The basis is returned as an array of sparse vectors, spvecgf2, called mcb.<br>
 *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
 *  numbering is represented by enumb.<br>
 *
 *  The function returns the weight of the approximate Minimum Cycle Basis or is undefined
 *  if there were any errors.<br> Even if the graph is directed this function computes an 
 *  approximate MCB of the underlying undirected graph.<br>
 *
 *  The running time is \f$O( m n^{1+1/k} + \min( m^3 + mn^2 \log n, n^{3+3/k}) )\f$ where \f$n\f$ are the number of nodes 
 *  of \f$g\f$, \f$m\f$ the number of edges and \f$k\f$ is an integer \f$\ge 1\f$.
 *
 *  \param g An undirected graph.
 *  \param k How much to approximate?
 *  \param mcb A leda::array of spvecgf2 to return the MCB.
 *  \param enumb An edge numbering.
 *  \return The length of the approximate MCB or undefined if some error occured.
 *  \pre g is loopfree.
 *  \pre len is non-negative
 *  \pre k must be an integer greater than zero
 *  \remark Care must be taken when the template parameter is instantiated with a data type
 *          which has rounding errors.
 *  \ingroup approxmcb
 */
int UMCB_APPROX( const graph& g,
	       const int k,
	       array< mcb::spvecgf2 >& mcb,
	       const mcb::edge_num& enumb
);

//@}

/*! \name Directed Approximate Minimum Cycle Basis
 */
//@{
/*! \brief Compute a directed approximate MCB of a weighted graph. 
 * 
 *  The function computes an \f$(2k-1)\f$-approximate Minimum Cycle
 *  Basis \f$B\f$ of a graph \f$g\f$.
 *  The basis is returned as an array of sparse vectors, spvecfp, called mcb.<br>
 *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
 *  numbering is represented by enumb.  
 *
 *  Since the algorithm is a randomized Monte-Carlo algorithm, the error argument 
 *  which should be less that 1 represents the acceptable error probability that the
 *  returned cycle basis is not a \f$(2k-1)\f$-approximate minimum cycle basis.
 *
 *  The function returns the weight of the approximate Minimum Cycle Basis or is undefined
 *  if there were any errors.<br>

 *  The running time is \f$O( mn^{1+1/k} + \min(m^3 + mn^2 \log n,n^{3+3/k}) )\f$ where \f$n\f$ are the number of 
 *  nodes of \f$g\f$, \f$m\f$ the number of edges 
 *  and \f$k\f$ is an integer \f$\ge 1\f$.
 *
 *  \param g An directed graph.
 *  \param len A leda::edge_array for the edge lengths.
 *  \param k How much to approximate?
 *  \param mcb A leda::array of spvecfp to return the approx MCB.
 *  \param enumb An edge numbering.
 *  \param error The error probability
 *  \return The length of the approximate MCB or undefined if some error occured.
 *  \pre g is loopfree.
 *  \pre len is non-negative
 *  \pre k must be an integer greater than zero
 *  \pre error is positive and less than one
 *  \remark Care must be taken when the template parameter is instantiated with a data type
 *          which has rounding errors.
 *  \ingroup approxmcb
 */
template<class W>
W DMCB_APPROX( const graph& g,
	       const edge_array<W>& len,
	       const int k,
	       array< mcb::spvecfp >& mcb,
	       const mcb::edge_num& enumb,
	       double error = 0.375
) 
{ 
    // check preconditions
#if ! defined(LEDA_CHECKING_OFF)
    if ( Is_Loopfree( g ) == false )
	error_handler(999,"MIN_CYCLE_BASIS: illegal graph (loops?)");
    if ( k <= 0 )
	error_handler(999,"MIN_CYCLE_BASIS: illegal value of k, non-positive?");

    edge e1;
    forall_edges( e1 , g ) { 
	if ( len[e1] < 0 ) 
	    error_handler(999,"MIN_CYCLE_BASIS: illegal edge (negative weight?)");
    }
#endif

    W length = W();
    int N = enumb.dim_cycle_space();
    mcb.resize( N ); // set size of destination array

#ifdef LEP_STATS
    float Tcycles = 0.0, Tsubgraph = 0.0, Ttemp;
    leda::used_time( Ttemp );
#endif
    
    // construct spanner and mappings
    graph spanner;
    node_map<node> node_g_to_spanner;
    node_map<node> node_spanner_to_g;
    edge_map<edge> edge_g_to_spanner;
    edge_map<edge> edge_spanner_to_g;

    mcb::SPANNER( g, len, k, spanner, 
	    node_g_to_spanner, node_spanner_to_g,
	    edge_g_to_spanner, edge_spanner_to_g,
	    enumb);

    // construct spanner edge lengths
    edge e, f;
    edge_array<W> spanner_len ( spanner );
    forall_edges( e, spanner ) { 
	spanner_len[ e ] = len[ edge_spanner_to_g[ e ] ];
    }

#ifdef LEP_STATS
    Tcycles += leda::used_time( Ttemp );
#endif

    // now compute the MCB of the spanner
    edge_num spanner_enumb( spanner );
    array< mcb::spvecfp > spanner_mcb, spanner_proof;

#ifdef LEP_DEBUG_OUTPUT
    std::cout << "Computing " << spanner_enumb.dim_cycle_space() << " cycles"; 
    std::cout << " by the MCB of spanner..." << std::endl;
#endif

    length += DMCB<W>( spanner, spanner_len, spanner_mcb, 
	    spanner_proof, spanner_enumb, error );

#ifdef LEP_STATS
    Tsubgraph += leda::used_time( Ttemp );
#endif

    // set prime number
    ptype p;
    if ( spanner_enumb.dim_cycle_space() > 0 ) 
	p = spanner_mcb[0].pvalue();
    else 
	p = 3;

    // now compute partial MCB cycles based on spanner
    ushortestpaths<W> usp( spanner, spanner_len );
    int i = 0;
    forall_edges( e, g ) { 
	if ( edge_g_to_spanner[e] == nil ) { 
	    mcb[i] = mcb::spvecfp( p );
	    
	    // compute shortest path on spanner
	    node spanner_s = node_g_to_spanner[ g.source(e) ];
	    node spanner_t = node_g_to_spanner[ g.target(e) ];
	    usp.compute_shortest_path( spanner_s, spanner_t );

#if ! defined(LEDA_CHECKING_OFF)
	    assert( usp.is_reachable( spanner_t ) && usp.pred( spanner_s ) == nil );
#endif

	    // form cycle
	    node spanner_w = spanner_t;
	    while( usp.pred( spanner_w ) != nil ) { 
		f = usp.pred( spanner_w );
		if ( spanner_w == spanner.source(f) ) {
		    mcb[i].append( enumb( edge_spanner_to_g[ f ] ), 1 );
		}
		else {
		    mcb[i].append( enumb( edge_spanner_to_g[ f ] ), -1 );
		}
		length += len[ edge_spanner_to_g[ f ] ];
		spanner_w = spanner.opposite( f, spanner_w );
	    }
	    mcb[i].append( enumb( e ) , 1 );
	    length += len[ e ];
	    mcb[i].sort(); // fix correct ordering

	    i++;
	}
    }

#ifdef LEP_STATS
    Tcycles += leda::used_time( Ttemp );
#endif
    
#ifdef LEP_DEBUG_OUTPUT
    std::cout << "Spanner has " << spanner.number_of_edges() << " edges..." << std::endl;
    std::cout << "Computed " << i << " cycles fast..." << std::endl;
#endif

    // now translate MCB of spanner to the original graph
    int extracycles = spanner_enumb.dim_cycle_space();
    for( i = 0; i < extracycles; ++i ) 
    { 
	// initialize
	mcb[ N - extracycles + i] = mcb::spvecfp( p );
	
	leda::list_item it = spanner_mcb[i].first();
	while( it != nil ) {
	    // translate to numbering of original graph 
	    // and add to cycle
	    e = edge_spanner_to_g[ spanner_enumb( spanner_mcb[i].index( it ) ) ];
	    mcb[ N - extracycles + i ].append( enumb(e), spanner_mcb[i].inf( it ) );
	    it = spanner_mcb[i].succ( it );
	}
	// fix ordering
	mcb[ N - extracycles + i ].sort();
    }

#ifdef LEP_STATS
    Tsubgraph += leda::used_time( Ttemp );
#endif

#ifdef LEP_STATS
    std::cout << "LEP_STATS: spanner cycles time: " << Tcycles << std::endl;
    std::cout << "LEP_STATS: subgraph MCB time  : " << Tsubgraph << std::endl;
#endif

    return length;
}
//@}



}

#endif  // MCB_APPROX_H

/* ex: set ts=8 sw=4 sts=4 noet: */


