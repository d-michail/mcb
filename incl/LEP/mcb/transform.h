
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

/*! \file transform.h
 *  \brief Transform cycle representations.
 *
 */

#ifndef TRANSFORM_H
#define TRANSFORM_H

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

    ///// UNDIRECTED CYCLES ///////

    /*! Transform a cycle from a sparse vector to a dynamic set.
     *  \param g The graph
     *  \param in A sparse vector
     *  \param out A dynamic set.
     */
    void spvecgf2_to_d_int_set( const graph& g,
	    const spvecgf2& in,
	    d_int_set& out );

    void d_int_set_to_spvecgf2( const graph& g,
	    const d_int_set& in,
	    spvecgf2& out );

    void spvecgf2_to_d_int_set( const graph& g,
	    const array< spvecgf2 >& in,
	    array< d_int_set >& out );

    /////// DIRECTED CYCLES //////

    void spvecfp_to_array_ints( const graph& g, const edge_num& enumb,
	    const ptype &p, const spvecfp& in, array<etype>& out );

    void array_ints_to_spvecfp( const graph& g, 
	    const edge_num& enumb, 
	    const ptype &p,
	    const array<etype>& in, 
	    spvecfp& out );


} // namespace mcb end

/* ex: set ts=8 sw=4 sts=4 noet: */



#endif  // TRANSFORM_H

/* ex: set ts=8 sw=4 sts=4 noet: */


