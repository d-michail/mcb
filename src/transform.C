
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

#include <LEP/mcb/config.h>

#ifdef LEDA_GE_V5
#include <LEDA/graph/graph.h>
#include <LEDA/graph/edge_array.h>
#include <LEDA/graph/graph_misc.h>
#include <LEDA/core/array.h>
#include <LEDA/core/list.h>
#include <LEDA/core/d_int_set.h>
#include <LEDA/system/error.h>
#else
#include <LEDA/graph.h>
#include <LEDA/array.h>
#include <LEDA/list.h>
#include <LEDA/edge_array.h>
#include <LEDA/graph_misc.h>
#include <LEDA/d_int_set.h>
#include <LEDA/error.h>
#include <LEDA/basic_graph_alg.h>
#endif

#include <LEP/mcb/spvecfp.h>
#include <LEP/mcb/spvecgf2.h>
#include <LEP/mcb/edge_num.h>


namespace mcb 
{ 

#if defined(LEDA_NAMESPACE)
using leda::graph;
using leda::node;
using leda::node_array;
using leda::node_set;
using leda::edge;
using leda::edge_array;
using leda::array;
using leda::d_int_set;
using leda::list;
using leda::error_handler;
#endif

///// UNDIRECTED CYCLES ///////

// from sparse vector to dynamic set
void spvecgf2_to_d_int_set( const graph& g,
	const spvecgf2& in,
	d_int_set& out )
{
    int index;
    out.clear();

    list_item it = in.first();
    while( it != nil ) {
	index = in.index( it );
#if ! defined(LEDA_CHECKING_OFF)
	assert( index >= 0 && index < g.number_of_edges() );
#endif
	out.insert( index );
	it = in.succ( it );
    }
}

void d_int_set_to_spvecgf2( const graph& g,
    const d_int_set& in,
    spvecgf2& out )
{
    out.clear();

    for( int i = 0; i < g.number_of_edges(); ++i )
    {
	if ( in.member(i) )
	    out.append( i );
    }
}

void spvecgf2_to_d_int_set( const graph& g,
	const array< spvecgf2 >& in,
	array< d_int_set >& out )
{
    out.resize( in.size() );
    for( int i = 0; i < in.size(); ++i ) {
	out[i] = d_int_set();
	spvecgf2_to_d_int_set( g, in[i], out[i] );
    }
}


/////// DIRECTED CYCLES //////

// from sparse vector to array of integers
void spvecfp_to_array_ints( const graph& g, const edge_num& enumb,
	const ptype &p, const spvecfp& in, array<etype>& out ) 
{
    int m = g.number_of_edges();
    if ( m <= 0 ) return;
    out.resize( m );
    out.init( 0 );

    indextype index;
    ptype tmp;

    list_item it = in.first();
    while( it != nil ) { 
	index = in.index( it );
#if ! defined(LEDA_CHECKING_OFF)
	assert( index >= 0 && index < m );
#endif
	tmp = in.inf( it );

	// normalize
	while ( tmp >= p ) tmp -= p;
	while ( tmp <= -p ) tmp += p;

	// record to -1, 0 or 1
	if ( tmp == -1 || tmp == p-1 ) out[index] = -1;
	else if ( tmp == 1 || tmp == -p+1 ) out[index] = 1;
	else if ( tmp == 0 ) out[index] = 0;
	else assert( false );

	it = in.succ( it );
    }
}  

void array_ints_to_spvecfp( const graph& g, 
	const edge_num& enumb, 
	const ptype &p,
	const array<etype>& in, 
	spvecfp& out )
{ 
    int m = g.number_of_edges();
    out.reset( p );

    indextype i;
    for( i = 0; i < m; i++ ) {
	if ( in[ i ] != 0 ) {
	    out.append( i, ptype(in[i]) );
	}
    }
}



} // namespace mcb end

/* ex: set ts=8 sw=4 sts=4 noet: */


