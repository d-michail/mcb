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


#ifndef MCB_H
#define MCB_H

#include <LEP/mcb/min_cycle_basis.h>
#include <LEP/mcb/dir_min_cycle_basis.h>

/*{\Mtext {\bf \Huge Minimum Cycle Basis}\medskip
   } */

// start our namespace
namespace mcb
{

    template < class W >
	W MCB( const graph & g, const edge_array < W > &len,
	       array < list < edge > >&mcb, double errorp = 0.375 )
	/*{\Mfunc $g$ is a graph and $len$ is a non-negative (positive for directed graphs) 
	   length function on the edges of $g$. The function computes a Minimum Cycle Basis $B$ of $g$, 
	   that is a cycle basis of $g$ with the smallest length (sum of lengths of cycles). The 
	   basis is returned as an array of lists of edges, called $mcb$. \\
	   The function returns the weight of the Minimum Cycle Basis or is undefined
	   if there were any errors. \\
	   The running time is $O( m^3 )$ where $m$ are the number of edges of $g$. \\
	   If the graph is directed the algorithm is a Monte-Carlo with success probability
	   of returning a minimum cycle basis, $5/8$. The algorithm always returns a basis.
	   If the graph is undirected, the minimum cycle basis is always returned. \\
	   The parameter $errorp$ can be used to decrease the error probability to something
	   less than $3/8$, by repeating the algorithm multiple times. It has effect only in the
	   directed case. \\
	   The space requirement is linear. \\
	   \precond $g$ simple and loopfree \\
	   \precond $len$ is non-negative for undirected graphs, positive for directed \\
	   \precond $errorp$ is positive and less than one \\
	   } */
    {
	if ( g.is_undirected(  ) ) {	// undirected graph
	    return MIN_CYCLE_BASIS_DEPINA( g, len, mcb );
	} else
	{			// directed graph
	    return DIR_MIN_CYCLE_BASIS( g, len, mcb, errorp );
	}
    }

    int MCB( const graph & g, array < list < edge > >&mcb, double errorp =
	     0.375 )
	/*{\Mfunc The same as above, but the graphs are unweighted. \\
	   \precond $g$ simple and loopfree \\
	   \precond $errorp$ is positive and less than one \\
	   } */
    {
	if ( g.is_undirected(  ) ) {	// undirected graph
	    return MIN_CYCLE_BASIS_DEPINA( g, mcb );
	} else {		// directed graph
	    leda::edge_array < int >len( g, 1 );

	    return DIR_MIN_CYCLE_BASIS( g, len, mcb, errorp );
	}
    }

}				// end of namespace mcb


#endif // MCB_H

/* ex: set ts=8 sw=4 sts=4 noet: */
