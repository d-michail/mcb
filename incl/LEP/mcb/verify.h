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


/*! \file verify.h
 *  \brief Algorithms for MCB verification
 *         
 *
 */

#ifndef VERIFY_H
#define VERIFY_H

#include <LEP/mcb/config.h>

#ifdef LEDA_GE_V5
#include <LEDA/graph/graph.h>
#include <LEDA/core/list.h>
#include <LEDA/core/d_int_set.h>
#include <LEDA/core/array.h>
#else
#include <LEDA/graph.h>
#include <LEDA/list.h>
#include <LEDA/d_int_set.h>
#include <LEDA/array.h>
#endif

#include <LEP/mcb/edge_num.h>
#include <LEP/mcb/spvecgf2.h>
#include <LEP/mcb/spvecfp.h>

// start our namespace
namespace mcb 
{ 

#if defined(LEDA_NAMESPACE)
using leda::graph;
using leda::array;
using leda::edge;
using leda::list;
using leda::d_int_set;
#endif

/*! Check that a set of edges is a cycle. Any set of edges is 
 *  considered to be a cycle as long as all vertex degrees are
 *  even.
 *
 *  \param g The input graph.
 *  \param enumb An edge numbering
 *  \param cycle A cycle.
 *  \return True if cycle is a cycle, false otherwise.
 */
bool verify_cycle( const graph& g,
	const edge_num& enumb,
	const spvecgf2& cycle
	);

/*! Check that several set of edges are a cycle. Any set of edges is 
 *  considered to be a cycle as long as all vertex degrees are
 *  even.
 *
 *  \param g The input graph.
 *  \param enumb An edge numbering
 *  \param cycles An array with cycles.
 *  \return True if cycles is an array of cycles, false otherwise.
 */
bool verify_cycles( const graph& g,
	const edge_num& enumb,
	const array< spvecgf2 >& cycles
	);

/*! Check that a set of edges is a cycle. Any set of edges is 
 *  considered to be a cycle as long as all vertex degrees are
 *  even.
 *
 *  \param g The input graph.
 *  \param enumb An edge numbering
 *  \param cycle A cycle.
 *  \return True if cycle is a cycle, false otherwise.
 */
bool verify_cycle( const graph& g,
	const edge_num& enumb,
	const spvecfp& cycle
	);
bool verify_cycles( const graph& g,
	const edge_num& enumb,
	const array< spvecfp >& cycles
	);


/*! Check that a set of edges is a cycle. Any set of edges is 
 *  considered to be a cycle as long as all vertex degrees are
 *  even.
 *
 *  \param g The input graph.
 *  \param enumb An edge numbering
 *  \param cycle A cycle.
 *  \return True if cycle is a cycle, false otherwise.
 */
bool verify_cycle( const graph& g,
	const edge_num& enumb,
	const d_int_set& cycle
	);

bool verify_cycles( const graph& g,
	const edge_num& enumb,
	const array< d_int_set >& cycles
	);

bool verify_basis( const graph& g,
	const mcb::edge_num& enumb,
	const array< d_int_set >& mcb);

bool verify_basis( const graph& g,
	const edge_num& enumb,
	const array< list<edge> >& mcb );

bool verify_basis( const graph& g,
	const edge_num& enumb,
	const array< spvecgf2 >& mcb );


/*! Check that a set of edges is a cycle. Any set of edges is 
 *  considered to be a cycle as long as all vertex degrees are
 *  even.
 *
 *  \param g The input graph.
 *  \param cycle A cycle.
 *  \return True if cycle is a cycle, false otherwise.
 */
bool verify_cycle( const leda::graph& g,
	const list<edge>& cycle
	);
bool verify_cycles( const leda::graph& g,
	const array< list<edge> >& cycles
	);


/*! Verify that a set of vectors are a cycle basis.
 *  \param g The graph
 *  \param enumb An edge numbering
 *  \param mcb The cycle basis
 *  \return True if cycle basis, false otherwise
 */
bool verify_cycle_basis( const graph& g,
	const mcb::edge_num& enumb,
	const array< d_int_set >& mcb
	);

/*! Verify that a set of vectors are a cycle basis.
 *  \param g The graph
 *  \param enumb An edge numbering
 *  \param mcb The cycle basis
 *  \return True if cycle basis, false otherwise
 */
bool verify_cycle_basis( const graph& g,
	const mcb::edge_num& enumb,
	const array< spvecgf2 >& mcb
	);

/*! Verify that a set of vectors are a cycle basis.
 *  \param g The graph
 *  \param enumb An edge numbering
 *  \param mcb The cycle basis
 *  \return True if cycle basis, false otherwise
 */
bool verify_cycle_basis( const graph& g,
	const mcb::edge_num& enumb,
	const array< list<edge> >& mcb
	);

bool verify_basis( const graph& g,
	const mcb::edge_num&,
	const array< spvecfp >& mcb);

bool verify_cycle_basis( const graph& g,
	const mcb::edge_num& enumb,
	const array< spvecfp >& mcb);


}

#endif  // VERIFY_H

/* ex: set ts=8 sw=4 sts=4 noet: */


