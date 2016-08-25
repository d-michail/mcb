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


#include <LEP/mcb/edge_num.h>
#include <LEP/mcb/fp.h>

namespace mcb
{

#if defined(LEDA_NAMESPACE)
    using leda::graph;
    using leda::edge;
    using leda::node;
    using leda::node_array;
    using leda::list;
    using leda::array;
#endif


    // from sparse vector to list of edges
    void spvecfp_to_list_edges( const graph & g, const edge_num & enumb,
				const spvecfp & in, list < edge > &out )
    {
	edge e;
	etype index;
	  out.clear(  );

	list_item it = in.first(  );
	while ( it != nil )
	{
	    index = in.index( it ) - 1;
#if ! defined(LEDA_CHECKING_OFF)
	    assert( index >= 0 && index < g.number_of_edges(  ) );
#endif
	    out.append( enumb( index ) );
	    it = in.succ( it );
	}
    }

    // from sparse vector to array of integers
    void spvecfp_to_array_ints( const graph & g, const edge_num & enumb,
				const ptype & p, const spvecfp & in,
				array < etype > &out )
    {
	int m = g.number_of_edges(  );

	if ( m <= 0 )
	    return;
	out.resize( m );
	out.init( 0 );

	edge e;
	indextype index;
	ptype tmp;

	list_item it = in.first(  );

	while ( it != nil ) {
	    index = in.index( it ) - 1;
#if ! defined(LEDA_CHECKING_OFF)
	    assert( index >= 0 && index < m );
#endif
	    tmp = in.inf( it );

	    // normalize
	    while ( tmp >= p )
		tmp -= p;
	    while ( tmp <= -p )
		tmp += p;

	    // record to -1, 0 or 1
	    if ( tmp == -1 || tmp == p - 1 )
		out[index] = -1;
	    else if ( tmp == 1 || tmp == -p + 1 )
		out[index] = 1;
	    else if ( tmp == 0 )
		out[index] = 0;
	    else
		assert( false );

	    it = in.succ( it );
	}
    }


    // verify that a sparse vector represents a cycle
    bool DMCB_verify_cycle( const graph & g,
			    const edge_num & enumb, const spvecfp & cycle )
    {
	if ( cycle.size(  ) == 0 )
	    return false;

	edge e;
	node v;
	etype index;

	node_array < etype > degree( g, 0 );

	// record degrees
	list_item it = cycle.first(  );

	while ( it != nil ) {
	    index = cycle.index( it ) - 1;
#if ! defined(LEDA_CHECKING_OFF)
	    assert( index >= 0 && index < g.number_of_edges(  ) );
#endif
	    e = enumb( index );
	    degree[g.target( e )]++;
	    degree[g.source( e )]++;
	    it = cycle.succ( it );
	}

	// check that degree are even
	forall_nodes( v, g )
	    if ( degree[v] % 2 == 1 )
	    return false;

	return true;
    }

    // check that all cycles of an array are indeed cycles
    // precondition, cycles array is C style [0..size-1]
    bool DMCB_verify_cycles( const graph & g,
			     const edge_num & enumb,
			     const array < spvecfp > &cycles )
    {
#if ! defined(LEDA_CHECKING_OFF)
	assert( cycles.C_style(  ) );
#endif
	for ( int i = 0; i < cycles.size(  ); i++ )
	    if ( DMCB_verify_cycle( g, enumb, cycles[i] ) == false )
		return false;
	return true;
    }

    // check that set of cycles are really a Basis
    bool DMCB_verify_basis( const graph & g,
			    const edge_num & enumb,
			    const array < spvecfp > &B,
			    const array < spvecfp > &X )
    {
	// check that we have cycles
	if ( DMCB_verify_cycles( g, enumb, B ) == false )
	    return false;

	int d = enumb.dim_cycle_space(  );

	if ( d < 1 )
	    return true;
	ptype p = B[0].pvalue(  );

	// check that matrix B * X is lower triangular with 
	// non-zero diagonial elements
	ptype v;

	for ( int i = 0; i < d; i++ ) {
	    for ( int j = i; j < d; j++ ) {
		v = B[i] * X[j];

		// bring to [0..p-1]
		while ( v >= p )
		    v -= p;
		while ( v < 0 )
		    v += p;

		if ( i == j ) {
		    if ( v == 0 )
			return false;
		} else {
		    if ( v != 0 )
			return false;
		}
	    }
	}

	// yes!!
	return true;
    }


}				// namespace mcb end

/* ex: set ts=8 sw=4 sts=4 noet: */
