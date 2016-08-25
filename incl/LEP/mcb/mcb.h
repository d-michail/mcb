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

/*! \file mcb.h
 *  \brief General LEP interface.
 */

/*! \mainpage Minimum Cycle Basis Library
 *
 *  \section intro_sec Introduction
 * 
 *  A minimum cycle basis is a basis of the cycle space of a graph 
 *  with minimum weight. The weight of a minimum cycle basis is the 
 *  sum of the weights of its cycles and the weight of a cycle is 
 *  the sum of the weights of its edges.
 *
 *  This package contains implementations of exact and approximate algorithms 
 *  to compute minimum cycle bases for weighted directed and undirected graphs. 
 * 
 *  \section alg_sec Algorithms
 *
 *  \subsection undir_alg_subsec Undirected Graphs
 *
 *  - An \f$O(m^3+mn^2 \log{n})\f$ algorithm which appeared in the PhD thesis of J.C. de Pina.
 *    A description of this algorithm and an even faster one can be 
 *    found 
 *    <a href="http://www.mpii.mpg.de/~michail/papers/MinCycleBasis.ps.gz">here</a>.
 *  - An \f$O(m^2n^2)\f$ hybrid algorithm, which is a mixture of the above 
 *    algorithm and an older algorithm due to Horton. For more details see 
 *    <a href="http://www.mpii.mpg.de/~michail/papers/implMCB.ps.gz">here</a>.
 *  - An \f$O(m^3)\f$ hybrid algorithm, which is a more clever implementation of 
 *    the last mentioned algorithm. For more details see this
 *    <a href="http://www.mpii.mpg.de/~michail/papers/mcblog.pdf">paper</a>.
 *  - An \f$O( m n^{1+1/k} + \min(m^3+mn^2 \log n, n^{3+3/k}))\f$ constant factor 
 *    \f$(2k-1)\f$-approximate algorithm. For more details see 
 *    <a href="http://www.springerlink.com/content/740w28k651vx26l8">this link</a>.
 *
 *  \subsection dir_alg_subsec Directed Graphs
 * 
 *  - An \f$O(m^3+mn^2 \log{n})\f$ implementation of an \f$O(m^2 n \log{n})\f$ 
 *    randomized Monte Carlo algorithm due to T. Kavitha, which appeared in ICALP'05.
 *  - An \f$O( m n^{1+1/k} + \min(m^3+mn^2 \log n, n^{3+3/k}))\f$ constant factor 
 *    \f$(2k-1)\f$-approximate algorithm. For more details see 
 *    <a href="http://www.springerlink.com/content/740w28k651vx26l8">this link</a>.
 *
 *  \section req_sec Requirements
 *  This implementation is written in C++ and uses 
 *  <a href="http://www.algorithmic-solutions.com/enleda.htm">LEDA</a>. 
 *  The structure of the package follows that of a LEDA extension package 
 *  (<a href="http://www.algorithmic-solutions.com/enleps.htm">LEP</a>).
 *
 *  \section platf_sec Supported Platforms
 *
 *  Some versions of the package have been tested on the following platforms:
 *  -# gcc 3.x, 4.0.x and 4.1.x under Linux 
 *  -# gcc 3.x under SunOS 5.9
 *  -# gcc 3.x under Cygwin
 *  -# bcc32 5.5 under Windows
 *  -# Visual Studio .NET 2003
 *
 *  but it may work on others too.
 *
 *  \section lic_sec License
 *
 *  \verbatim
    This program can be freely used in an academic environment
    ONLY for research purposes, subject to the following restrictions:
 
    1. The origin of this software must not be misrepresented; you must not
       claim that you wrote the original software. If you use this software
       an acknowledgment in the product documentation is required.
    2. Altered source versions must be plainly marked as such, and must not be
       misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
 
    Any other use is strictly prohibited by the author, without an explicit
    permission.
 
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    \endverbatim
 *
 *  Note that this package uses LEDA, which is 
 *  <a href="http://www.algorithmic-solutions.com/enledalizenzen.htm">not free</a>.
 *  However, Algorithmic Solutions released a 
 *  <a href="http://www.algorithmic-solutions.com/leda/ledak/index.htm">free version</a> 
 *  of LEDA 6.0 which can be used to build this library.
 *
 *  \section news_sec News
 *
 *  - 18 April 2008: v0.8 released
 *      - LEDA 6.0 and LEDA 6.0 free support
 *      - Interface change (not compatible with previous versions)
 *      - A lot of improvements and new features.
 *      - Improved demo program.
 *  - 1  December 2006: v0.7 released
 *      - Constant factor approximation algorithms for undirected and 
 *        directed graphs.
 *      - Interface change (not compatible with previous versions)
 *  - 30 March 2006: v0.6 released
 *      - Support for Microsoft VC++ 7.1 compiler.
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
 *  - Source package (v0.8). [<a href="../mcb-0.8.tar.gz">tar.gz</a>]
 *  - Source package (v0.7). [<a href="../mcb-0.7.tar.gz">tar.gz</a>]
 *  - Source package (v0.6). [<a href="../mcb-0.6.tar.gz">tar.gz</a>]
 *  - Source package (v0.5). [<a href="../mcb-0.5.tar.gz">tar.gz</a>]
 *  - Source package (v0.4). [<a href="../mcb-0.4.tar.gz">tar.gz</a>]
 *  - Source package (v0.3). [<a href="../mcb-0.3.tar.gz">tar.gz</a>]
 *  - Source package (v0.2). [<a href="../mcb-0.2.tar.gz">tar.gz</a>]
 *  - Source package (v0.1). [<a href="../mcb-0.1.tar.gz">tar.gz</a>]
 *
 *  \section ex_sec Code Examples
 *  \subsection ex_undir Undirected MCB
 *  \code
 *  #include <LEP/mcb/mcb.h>
 *
 *  int main() {
 *
 *      leda::graph G;
 *
 *      // construct undirected, loopfree graph G 
 *
 *      leda::edge_array<int> len(G, 1);
 *
 *      // fill up non-negative edge lengths 
 *
 *      mcb::edge_num enumb( G );
 *      leda::array< mcb::spvecgf2 > mcb;
 *      int weight = mcb::UMCB( G, len, mcb, enumb ); 
 *
 *      int i,j;
 *      leda::edge e;
 *      for( i = 0; i < enumb.dim_cycle_space(); ++i ) {
 *              forall( j, mcb[i] ) { // traverse edges of i-th cycle
 *                      e = enumb( j );
 *
 *                      // do something with edge e 
 *              }
 *      }
 *  } 
 *  \endcode
 *
 * \subsection ex_dir Directed MCB
 * \code
 * #include <LEP/mcb/mcb.h>
 *
 * int main() {
 *
 *      leda::graph G;
 *
 *      // construct simple, loopfree, directed graph G 
 *
 *      leda::edge_array<int> len(G, 1);
 *
 *      // fill up positive edge lengths 
 *
 *      leda::array< mcb::spvecfp > mcb;
 *      int weight = mcb::DMCB( G, len, mcb );
 *
 *      int i;
 *      leda::edge e;
 *      ptype direction;
 *      for( i = 0; i < enumb.dim_cycle_space(); ++i ) {
 *              leda::list_item it = mcb[i].first();
 *              while( it != nil ) {
 *                     e = enumb( mcb[i].index( it ) );
 *                     direction = mcb[i].inf( it );
 *
 *                     // do something with edge e
 *                     // direction is -1 or 1 based on traversing the cycle
 *                     // in some arbitrary direction
 *
 *                     it = mcb[i].succ( it );
 *              }
 *      }
 * }
 * \endcode
 *  \subsection ex_approx Undirected 2k-1 Approximate MCB
 *  \code
 *  #include <LEP/mcb/mcb.h>
 *
 *  int main() {
 *
 *      leda::graph G;
 *      // construct loopfree graph G 
 *
 *      leda::edge_array<int> len(G, 1);
 *      // fill up non-negative edge lengths 
 *
 *      // setup constant for approximation factor 2k-1
 *      int k = 2; 
 *
 *      mcb::edge_num enumb( G );
 *      leda::array< mcb::spvecgf2 > mcb;
 *      int weight = mcb::UMCB_APPROX( G, len, k, mcb, enumb ); 
 *
 *      int i,j;
 *      leda::edge e;
 *      for( i = 0; i < enumb.dim_cycle_space(); ++i ) {
 *              forall( j, mcb[i] ) { // traverse edges of i-th cycle
 *                      e = enumb( j );
 *
 *                      // do something with edge e 
 *              }
 *      }
 *  } 
 *  \endcode
 *
 */

/*! \namespace mcb 
 *  \brief The main package namespace
 */
/*! \defgroup approxmcb Approximate MCB
 *  \defgroup exactmcb  Exact MCB
 *  \defgroup mcbutil   Various Utilities
 */

#ifndef MCB_H
#define MCB_H

#include <LEP/mcb/umcb.h>
#include <LEP/mcb/spanner.h>
#include <LEP/mcb/dmcb.h>
#include <LEP/mcb/mcb_approx.h>

#endif  // MCB_H

/* ex: set ts=4 sw=4 sts=4 et: */
