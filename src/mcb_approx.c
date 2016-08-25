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


/*! \file mcb_approx.C
 *  \brief Approximate MCB construction.
 */

#include <LEP/mcb/spanner.h>
#include <LEP/mcb/ushortpath.h>
#include <LEP/mcb/mcb_approx.h>

namespace mcb { 

#if defined(LEDA_NAMESPACE)
using leda::graph;
using leda::edge;
using leda::edge_map;
using leda::edge_array;
using leda::node;
using leda::node_map;
using leda::node_array;
#endif

// compute approximate MCB of undirected unweighted graph
int UMCB_APPROX( const graph& g,
	       const int k,
	       array< mcb::spvecgf2 >& mcb,
	       const edge_num& enumb
) 
{ 
    // check preconditions
#if ! defined(LEDA_CHECKING_OFF)
    if ( Is_Loopfree( g ) == false )
	error_handler(999,"MIN_CYCLE_BASIS: illegal graph (loops?)");
    if ( k <= 0 )
	error_handler(999,"MIN_CYCLE_BASIS: illegal value of k, non-positive?");
#endif

    int length = 0;
    int N = enumb.dim_cycle_space();
    mcb.resize( N ); // set size of destination array


#ifdef LEP_STATS
    float Tcycles = 0.0, Tsubgraph = 0.0, Ttemp;
#endif

#ifdef LEP_STATS
    leda::used_time( Ttemp );
#endif
    
    // construct spanner and mappings
    graph spanner;
    node_map<node> node_g_to_spanner;
    node_map<node> node_spanner_to_g;
    edge_map<edge> edge_g_to_spanner;
    edge_map<edge> edge_spanner_to_g;
    edge_array<int> len(g, 1);

    mcb::SPANNER( g, len, k, spanner, 
	    node_g_to_spanner, node_spanner_to_g,
	    edge_g_to_spanner, edge_spanner_to_g,
	    enumb);

#ifdef LEP_DEBUG_OUTPUT
    std::cout << "Spanner has " << spanner.number_of_edges() << " edges." << std::endl;
#endif

    // now compute partial MCB cycles based on spanner
    ubfs usp( spanner );
    int i = 0;
    edge e,f;
    forall_edges( e, g ) {
	if ( edge_g_to_spanner[e] == nil ) { 
	    mcb[i] = mcb::spvecgf2();

	    // compute shortest path on spanner
	    node spanner_s = node_g_to_spanner[ g.source(e) ];
	    node spanner_t = node_g_to_spanner[ g.target(e) ];
	    usp.compute_shortest_path( spanner_s, spanner_t, 2*k-1 );

	    assert( usp.is_reachable( spanner_t ) && usp.pred( spanner_s ) == nil );

	    // form cycle
	    node spanner_w = spanner_t;
	    while( usp.pred( spanner_w ) != nil ) {
		f = usp.pred( spanner_w );
		mcb[i].insert( enumb( edge_spanner_to_g[ f ] ) );
		++length;
		spanner_w = spanner.opposite( f, spanner_w );
	    }
	    mcb[i].insert( enumb(e) );
	    ++length;
	    mcb[i].sort(); 

	    ++i;
	}
    }

#ifdef LEP_STATS
    Tcycles += leda::used_time( Ttemp );
#endif

    // now compute the MCB of subgraph
    edge_num spanner_enumb( spanner );
    array< spvecgf2 > spanner_mcb, spanner_proof;

    length += MIN_CYCLE_BASIS_DEPINA( spanner, 
	    spanner_mcb, spanner_proof, spanner_enumb );

#ifdef LEP_DEBUG_OUTPUT
        std::cout << "Computing " << spanner_enumb.dim_cycle_space() << " cycles";
	std::cout << " by the spanner's MCB..." << std::endl;
#endif
    
    // now translate to the original graph
    int extracycles = spanner_enumb.dim_cycle_space();
    for( int i = 0; i < extracycles; ++i ) 
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
#endif

#ifdef LEP_STATS
    std::cout << "LEP_STATS: spanner cycles time: " << Tcycles << std::endl;
    std::cout << "LEP_STATS: subgraph MCB time  : " << Tsubgraph << std::endl;
#endif

    return length;
}

} // end namespace mcb

/* ex: set ts=8 sw=4 sts=4 noet: */
