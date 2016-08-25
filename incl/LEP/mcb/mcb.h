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


/*! \file mcb.h
 *  \brief General LEP interface.
 */

/*! \mainpage Minimum Cycle Basis Leda Extension Package
 *
 *  \section intro_sec Introduction
 * 
 *  A minimum cycle basis is a basis of the cycle space of a graph 
 *  with minimum weight. The weight of a minimum cycle basis is the 
 *  sum of the weights of its cycles and the weight of a cycle is 
 *  the sum of the weights of its edges.
 *
 *  This package contains implementations of algorithms to compute 
 *  minimum cycle bases for weighted directed and undirected graphs. 
 * 
 *  \section alg_sec Algorithms
 *
 *  \subsection undir_alg_subsec Undirected Graphs
 *
 *  - An \f$O(m^3+mn^2 \log{n})\f$ algorithm due to J.C. de Pina.
 *    A description of this algorithm and an even faster one can be 
 *    found 
 *    <a href="http://www.mpii.mpg.de/~michail/papers/MinCycleBasis.ps.gz">here</a>.
 *  - An \f$O(m^2n^2)\f$ hybrid algorithm, which is a mixture of the above 
 *    algorithm and an older algorithm due to Horton. For more details see 
 *    <a href="http://www.mpii.mpg.de/~michail/papers/implMCB.ps.gz">here</a>.
 *
 *  \subsection dir_alg_subsec Directed Graphs
 * 
 *  - An \f$O(m^3+mn^2 \log{n})\f$ implementation of an \f$O(m^2 n \log{n})\f$ 
 *    algorithm due to T. Kavitha, which appeared in ICALP'05.
 *
 *  \section req_sec Requirements
 *  This implementation is written in C++ and uses 
 *  <a href="http://www.algorithmic-solutions.com/enleda.htm">LEDA</a>. 
 *  The structure of the package follows that of a LEDA extension package 
 *  (<a href="http://www.algorithmic-solutions.com/enleps.htm">LEP</a>).
 *
 *  \section platf_sec Supported Platforms
 *  This package has been tested on the following platforms:
 *  -# gcc 3.x under Linux, SunOS 5.9, Cygwin
 *  -# bcc32 5.5 under Windows
 *
 *  but it may work on others too.
 *
 *  \section lic_sec License
 *
 *  <pre>
 *  This program can be freely used in an academic environment
 *  ONLY for research purposes, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     an acknowledgment in the product documentation is required.
 *  2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *  3. This notice may not be removed or altered from any source distribution.
 *
 *  Any other use is strictly prohibited by the author, without an explicit
 *  permission.
 *
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  </pre>
 *
 *  Note that this package uses LEDA, which is 
 *  <a href="http://www.algorithmic-solutions.com/enledalizenzen.htm">not free</a>.
 *
 *  \section news_sec News
 * 
 *  - 27 Oct 2005: v0.5 released
 *      - Speed improvements.
 *      - Build system improvements.
 *  - 30 June 2005: v0.4 released
 *      - The code now is by default compiled with -O2 optimization flag.
 *      - Minor fixes.
 *  - 30 May 2005: v0.3 released
 *      - Added algorithm for the directed case, still in BETA phase.
 *      - Improved support for LEDA 5.0, including support on windows platform.
 *  - 21 Mar 2005: v0.2 released
 *      - minor changes in demo program.
 *  - 24 Feb 2005: v0.1 released
 *
 *  \section down_sec Download
 *
 *  - Source package (v0.5). [<a href="../mcb-0.5.tar.gz">tar.gz</a>]
 *  - Source package (v0.4). [<a href="../mcb-0.4.tar.gz">tar.gz</a>]
 *  - Source package (v0.3). [<a href="../mcb-0.3.tar.gz">tar.gz</a>]
 *  - Source package (v0.2). [<a href="../mcb-0.2.tar.gz">tar.gz</a>]
 *  - Source package (v0.1). [<a href="../mcb-0.1.tar.gz">tar.gz</a>]
 *
 *  \section ex_sec Code Examples
 *  \subsection Undirected
 *  \code
 *  #include <LEP/mcb/min_cycle_basis.h>
 *  int main() {
 *
 *      leda::graph G;
 *
 *      // construct simple, loopfree, undirected graph G 
 *
 *      leda::edge_array<int> len(G, 1);
 *
 *      // fill up non-negative edge lengths 
 *
 *      mcb::edge_num enumb( G );
 *      leda::array< mcb::spvecgf2 > mcb;
 *      int weight = mcb::MIN_CYCLE_BASIS_DEPINA( G, len, mcb, enumb ); 
 *
 *      int i,j;
 *      leda::edge e;
 *      for( i = 0; i < enumb.dim_cycle_space(); i++ ) {
 *              forall( j, mcb[i] ) { // traverse edges of i-th cycle
 *                      e = enumb( j );
 *
 *                      // do something with edge e 
 *              }
 *      }
 *  } 
 *  \endcode
 *
 * \subsection ex_dir Directed
 * \code
 * int main() {
 *
 *   leda::graph G;
 *
 *   // construct simple, loopfree, directed graph G 
 *
 *   leda::edge_array<int> len(G, 1);
 *
 *   // fill up positive edge lengths 
 *
 *   leda::array< leda::list<leda::edge> > mcb;
 *   int weight = mcb::DIR_MIN_CYCLE_BASIS( G, len, mcb );
 * }
 * \endcode
 *
 */

#ifndef MCB_H
#define MCB_H

#include <LEP/mcb/min_cycle_basis.h>
#include <LEP/mcb/dir_min_cycle_basis.h>

/*! \namespace mcb
 *  \brief Main namespace of MCB LEDA Extension Package.
 */
namespace mcb 
{ 

/*! \brief Compute a minimum cycle basis of a graph.
 *
 * The function computes a Minimum Cycle Basis \f$B\f$ of \f$g\f$, 
 *  that is a cycle basis of \f$g\f$ with the smallest length (sum of lengths of cycles). The 
 *  basis is returned as an array of lists of edges, called mcb.<br>
 *  The running time is \f$O( m^3 )\f$ where \f$m\f$ are the number of edges of \f$g\f$.
 *  If the graph is directed the algorithm is a Monte-Carlo with success probability
 *  of returning a minimum cycle basis, \f$5/8\f$. The algorithm always returns a basis.
 *  If the graph is undirected, the minimum cycle basis is always returned.<br>
 *  The parameter \f$errorp\f$ can be used to decrease the error probability to something
 *  less than \f$3/8\f$, by repeating the algorithm multiple times. It has effect only in the
 *  directed case.<br>
 *  The space requirement is linear.
 *
 *  \param g The graph.
 *  \param len An edge array with the edge lengths.
 *  \param mcb An array of list of edges to store the MCB.
 *  \param errorp Error parameter, only used in the directed case.
 *  \return The length of the MCB if the MCB was computed successfuly, otherwise undefined.
 *  \pre len is non-negative for undirected and positive for directed graphs.
 *  \pre g is simple and loopfree
 *  \pre errorp is a valid probability larger than zero
 */
template<class W>
W MCB( const leda::graph& g, const leda::edge_array<W>& len, 
       leda::array< leda::list<leda::edge> >& mcb, double errorp=0.375 ) 
{
    if ( g.is_undirected() ) { // undirected graph
        return MIN_CYCLE_BASIS_DEPINA( g, len, mcb );
    }
    else { // directed graph
        return DIR_MIN_CYCLE_BASIS( g, len, mcb, errorp );
    }
}

/*! \brief Compute a minimum cycle basis of an unweighted graph.
 *
 * The function computes a Minimum Cycle Basis \f$B\f$ of \f$g\f$, 
 *  that is a cycle basis of \f$g\f$ with the smallest length (sum of lengths of cycles). The 
 *  basis is returned as an array of lists of edges, called mcb.<br>
 *  The running time is \f$O( m^3 )\f$ where \f$m\f$ are the number of edges of \f$g\f$.
 *  If the graph is directed the algorithm is a Monte-Carlo with success probability
 *  of returning a minimum cycle basis, \f$5/8\f$. The algorithm always returns a basis.
 *  If the graph is undirected, the minimum cycle basis is always returned.<br>
 *  The parameter \f$errorp\f$ can be used to decrease the error probability to something
 *  less than \f$3/8\f$, by repeating the algorithm multiple times. It has effect only in the
 *  directed case.<br>
 *  The space requirement is linear.
 *
 *  \param g The graph.
 *  \param mcb An array of list of edges to store the MCB.
 *  \param errorp Error parameter, only used in the directed case.
 *  \return The length of the MCB if the MCB was computed successfuly, otherwise undefined.
 *  \pre g is simple and loopfree
 *  \pre errorp is a valid probability larger than zero
 */
int MCB( const graph& g, array< list<edge> >& mcb, double errorp = 0.375 ) 
{
    if ( g.is_undirected() ) { // undirected graph
        return MIN_CYCLE_BASIS_DEPINA( g, mcb );
    }
    else { // directed graph
        leda::edge_array<int> len( g, 1);
        return DIR_MIN_CYCLE_BASIS( g, len, mcb, errorp );
    }
}
    
} // end of namespace mcb


#endif  // MCB_H

/* ex: set ts=8 sw=4 sts=4 noet: */


