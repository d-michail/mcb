
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


#ifndef EDGE_NUM_H
#define EDGE_NUM_H

#ifdef LEDA_GE_V5
#include <LEDA/graph/graph.h>
#include <LEDA/graph/edge_array.h>
#include <LEDA/graph/node_set.h>
#include <LEDA/core/b_queue.h>
#else
#include <LEDA/graph.h>
#include <LEDA/edge_array.h>
#include <LEDA/node_set.h>
#include <LEDA/b_queue.h>
#endif

namespace mcb
{


    /*{\Mtext \newpage {\bf \Large Edge Numbering (edge\_num)}\medskip} */
    /*{\Manpage {edge_num} {} {An edge numbering} {l}} */

    class edge_num
    {
	/*{\Mdefinition
	   A numbering |\Mvar| provides a numbering of the edges of a graph 
	   from $0$ to $m-1$. The numbering is based on an arbitrary spanning tree $T$.
	   Edges not in $T$ are numbered from $0$ to $m -n + \kappa -1$ where $\kappa$
	   are the number of (weakly) connected components of $G$. Edges in $T$ are numbered
	   from $m-n+\kappa$ to $m-1$.} */

      private:
	// variables:
	leda::edge * index;
	leda::edge_array < int >rindex;
	int m, n, k;
	// methods:
	void create_numbering( const leda::graph & G );
	int construct_tree( const leda::graph & g,
			    leda::edge_array < bool > &tree );

	// disabled:
	  edge_num & operator=( const edge_num & );

      public:
	/*{\Mcreation
	   } */
	  edge_num( const leda::graph & G );
	/*{\Mcreate creates a |\Mname| for a graph |G|.
	   } */
	  edge_num( const edge_num & enumb );
	/*{\Mcreate creates a |\Mname| from an edge numbering $enumb$.
	   } */
	 ~edge_num( void );


	/*{\Moperations
	   } */
	inline int operator(  ) ( leda::edge e ) const
	{
	    return rindex[e];
	}
	/*{\Mfunop Takes edge |e| and returns its number.
	   } */

	inline leda::edge operator(  ) ( int i ) const
	{
#if ! defined(LEDA_CHECKING_OFF)
	    if ( i < 0 || i > m )
		leda::error_handler( 999,
				     "edge_num: illegal number requested" );
#endif
	    return index[i];
	}
	/*{\Mfunop Takes integer |i| and returns an edge with this number.
	   } */



	bool tree( leda::edge e ) const
	{
	    return ( rindex[e] >= m - n + k );
	}
	/*{\Mfunc Returns whether an edge |e| belongs to the tree used for constructing
	   the numbering.} */

	int dim_cycle_space(  ) const
	{
	    return m - n + k;
	}
	/*{\Mfunc Returns the dimension of the cycle space. More precisely |m-n+k|, where
	   |k| is the number of (weakly) connected components of the graph.} */

	int num_weak_connected_comp(  ) const
	{
	    return k;
	}
	/*{\Mfunc Returns the number of (weakly) connected components of the graph.
	   } */

	/*{\Mimplementation
	   The |\Mname| is implemented as two arrays, and therefore requires
	   $O(m)$ space. All operations take constant time except construction
	   which takes linear time.} */
    };


}				// end namespace mcb

#endif

/* ex: set ts=8 sw=4 sts=4 noet: */
