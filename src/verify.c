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

#include <LEP/mcb/config.h>

#ifdef LEDA_GE_V5
#include <LEDA/graph/graph.h>
#include <LEDA/core/array.h>
#include <LEDA/core/list.h>
#include <LEDA/core/d_int_set.h>
#include <LEDA/system/error.h>
#include <LEDA/numbers/integer_matrix.h>
#else
#include <LEDA/graph.h>
#include <LEDA/array.h>
#include <LEDA/list.h>
#include <LEDA/d_int_set.h>
#include <LEDA/error.h>
#include <LEDA/integer_matrix.h>
#endif

#include <LEP/mcb/fp.h>
#include <LEP/mcb/spvecgf2.h>
#include <LEP/mcb/edge_num.h>
#include <LEP/mcb/transform.h>


namespace mcb 
{ 

#if defined(LEDA_NAMESPACE)
using leda::graph;
using leda::node;
using leda::node_array;
using leda::edge;
using leda::array;
using leda::d_int_set;
using leda::list;
using leda::error_handler;
#endif


////// VERIFY CYCLES ///////

bool verify_cycle( const graph& g,
		   const edge_num& enumb,
		   const spvecgf2& cycle
		 )
{
    if ( cycle.size() == 0 ) return false;

    edge e;
    indextype index;
    node_array<indextype> degree( g, 0 );
    
    // count degrees
    leda::list_item it = cycle.first();
    while( it != nil ) 
    {
	index = cycle.index( it );
#if ! defined(LEDA_CHECKING_OFF)
        assert( index >= 0 && index < g.number_of_edges() );
#endif
	e = enumb( index );
	degree[ g.target(e) ]++;
	degree[ g.source(e) ]++;
	it = cycle.succ( it );
    }

    // check that even degrees
    node v;
    forall_nodes( v, g )
	if ( degree[ v ] % 2 == 1 ) 
	    return false;

    return true;
}

bool verify_cycles( const graph& g,
		   const edge_num& enumb,
		   const array< spvecgf2 >& cycles
		 )
{
#if ! defined(LEDA_CHECKING_OFF)
    assert( cycles.C_style() );
#endif
    for( int i = 0; i < cycles.size(); ++i )
	if ( verify_cycle( g, enumb, cycles[i] ) == false )
	    return false;
    return true;
}


bool verify_cycle( const graph& g,
		   const edge_num& enumb,
		   const spvecfp& cycle
		)
{
    if ( cycle.size() == 0 ) return false;

    edge e;
    indextype index;
    node_array<indextype> degree( g, 0 );
    
    // count degrees
    leda::list_item it = cycle.first();
    while( it != nil ) 
    {
	index = cycle.index( it );
#if ! defined(LEDA_CHECKING_OFF)
        assert( index >= 0 && index < g.number_of_edges() );
#endif
	e = enumb( index );
	degree[ g.target(e) ]++;
	degree[ g.source(e) ]++;
	it = cycle.succ( it );
    }

    // check that even degrees
    node v;
    forall_nodes( v, g )
	if ( degree[ v ] % 2 == 1 ) 
	    return false;

    return true;
}

bool verify_cycles( const graph& g,
		   const edge_num& enumb,
		   const array< spvecfp >& cycles
		)
{
#if ! defined(LEDA_CHECKING_OFF)
    assert( cycles.C_style() );
#endif
    for( int i = 0; i < cycles.size(); ++i )
	if ( verify_cycle( g, enumb, cycles[i] ) == false )
	    return false;
    return true;
}


bool verify_cycle( const graph& g,
		   const edge_num& enumb,
		   const d_int_set& cycle
		)
{
    if ( cycle.size() == 0 || cycle.empty() == true ) 
	return false;

    edge e;
    node_array<indextype> degree( g, 0 );

    for( int i = 0; i < g.number_of_edges(); ++i ) 
    {
	if ( cycle.member( i ) ) 
	{
	    e = enumb( i );
	    degree[ g.target(e) ]++;
	    degree[ g.source(e) ]++;
	}
    }
    
    // check that even degrees
    node v;
    forall_nodes( v, g )
	if ( degree[ v ] % 2 == 1 ) 
	    return false;

    return true;
}

bool verify_cycles( const graph& g,
		   const edge_num& enumb,
		   const array< d_int_set >& cycles
		)
{
#if ! defined(LEDA_CHECKING_OFF)
    assert( cycles.C_style() );
#endif
    for( int i = 0; i < cycles.size(); ++i )
	if ( verify_cycle( g, enumb, cycles[i] ) == false )
	    return false;
    return true;
}

bool verify_cycle( const graph& g,
		   const list<edge>& cycle
		)
{
    if ( cycle.empty() ) 
	return false;

    edge e;
    node_array<indextype> degree( g, 0 );

    forall( e, cycle ) 
    {
	degree[ g.target(e) ]++;
	degree[ g.source(e) ]++;
    }
    
    // check that even degrees
    node v;
    forall_nodes( v, g )
	if ( degree[ v ] % 2 == 1 ) 
	    return false;

    return true;
}

bool verify_cycles( const graph& g,
		   const array< list<edge> >& cycles
		)
{
#if ! defined(LEDA_CHECKING_OFF)
    assert( cycles.C_style() );
#endif
    for( int i = 0; i < cycles.size(); ++i )
	if ( verify_cycle( g, cycles[i] ) == false )
	    return false;
    return true;
}



////// VERIFY BASIS UNDIRECTED //////////

bool verify_basis( const graph& g, 
                   const mcb::edge_num& enumb,
                   const array< d_int_set >& mcb)
{
#if ! defined(LEDA_CHECKING_OFF)
    assert( mcb.C_style() );
#endif

    if ( mcb.size() == 0 ) return true;

    // now check for linear independence
    // copy cycles
    array< d_int_set > a(mcb.size());
    for( int i = 0; i < mcb.size(); ++i ) a[i] = mcb[i];

    // do gauss elimination
    for( int i = 0; i < a.size(); ++i) {
	// find non-zero
	if ( a[i].member( i ) == false ) {
	    for( int j = i+1; j < a.size(); ++j ) { 
		if ( a[j].member(i) == true ) {
		    d_int_set Stemp = a[i];
		    a[i] = a[j]; 
		    a[j] = Stemp;
		}
	    }
	}

	// no non-zero found
	if ( a[i].member( i ) == false ) return false;

	// 
	for( int j = i + 1; j < a.size(); ++j ) { 
	    if ( a[j].member( i ) == true ) 
		a[j] %= a[i];
	}
    }

    return true;
}

bool verify_basis( const graph& g, 
	const edge_num& enumb, 
	const array< list<edge> >& mcb )
{
    array< d_int_set > mcb_temp;
    list_edges_to_d_int_set( g, enumb, mcb, mcb_temp );
    return verify_basis( g, enumb, mcb_temp );
}

bool verify_basis( const graph& g, 
	const edge_num& enumb, 
	const array< spvecgf2 >& mcb )
{
    array< d_int_set > mcb_temp;
    spvecgf2_to_d_int_set( g, mcb, mcb_temp );
    return verify_basis( g, enumb, mcb_temp );
}


bool verify_cycle_basis( const graph& g, 
                   const mcb::edge_num& enumb,
                   const array< d_int_set >& mcb)
{
#if ! defined(LEDA_CHECKING_OFF)
    assert( mcb.C_style() );
#endif

    if ( mcb.size() != enumb.dim_cycle_space() )
	return false;

    if ( verify_cycles( g, enumb, mcb ) == false ) 
	return false;

    return verify_basis( g, enumb, mcb );
}

bool verify_cycle_basis( const graph& g, 
                   const mcb::edge_num& enumb,
                   const array< list<edge> >& mcb)
{
#if ! defined(LEDA_CHECKING_OFF)
    assert( mcb.C_style() );
#endif

    if ( mcb.size() != enumb.dim_cycle_space() )
	return false;

    if ( verify_cycles( g, mcb ) == false ) 
	return false;

    return verify_basis( g, enumb, mcb );
}

bool verify_cycle_basis( const graph& g, 
		   const edge_num& enumb,
                   const array< spvecgf2 >& mcb)
{
#if ! defined(LEDA_CHECKING_OFF)
    assert( mcb.C_style() );
#endif

    if ( mcb.size() != enumb.dim_cycle_space() )
	return false;

    if ( verify_cycles( g, enumb, mcb ) == false ) 
	return false;

    return verify_basis( g, enumb, mcb );
}



////// VERIFY BASIS DIRECTED //////////
bool verify_basis( const graph& g, 
                   const mcb::edge_num&,
                   const array< spvecfp >& mcb)
{
#if ! defined(LEDA_CHECKING_OFF)
    assert( mcb.C_style() );
#endif

    if ( mcb.size() == 0 ) return true;

    // use LEDA matrix and rank function
    leda::integer_matrix a( mcb.size(), g.number_of_edges() );
    for( int i = 0; i < mcb.size(); ++i ) 
    {
	for( int j = 0; j < g.number_of_edges(); ++j ) 
	    a( i, j ) = 0;
	leda::list_item it = mcb[i].first();
	while( it != nil ) 
	{
	    a( i, mcb[i].index( it ) ) = mcb[i].inf( it );
	    it = mcb[i].succ( it );
	}
    }

    int r = leda::rank(a);
#ifdef LEP_DEBUG_OUTPUT
    std::cout << "verify basis: rank = " << r << std::endl;
#endif
    return ( r == mcb.size() );
}

bool verify_cycle_basis( const graph& g,
	const mcb::edge_num& enumb,
	const array< spvecfp >& mcb)
{
#if ! defined(LEDA_CHECKING_OFF)
    assert( mcb.C_style() );
#endif

    if ( mcb.size() != enumb.dim_cycle_space() )
	return false;

    if ( verify_cycles( g, enumb, mcb ) == false ) 
	return false;

    return verify_basis( g, enumb, mcb );
}



} // namespace mcb end

/* ex: set ts=8 sw=4 sts=4 noet: */


