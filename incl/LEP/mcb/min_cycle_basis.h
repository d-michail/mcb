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


/*! \file min_cycle_basis.h
 *  \brief Algorithms for undirected MCB.
 *
 *  Given an undirected graph \f$G(V,E)\f$ and a non-negative length function on the edges
 *  \f$w: E \mapsto \mathcal{R}_{\ge 0}\f$, a minimum cycle basis is a set of cycles
 *  which can generate the cycle space and at the same time has minimum total length.
 *
 *  Each cycle of the graph is assumed to be a 0-1 vector indexed on the edge set, and
 *  operations between cycles is performed in GF(2). The length of a cycle basis is 
 *  the sum of the length of its cycles and the length of a cycle is the sum of the 
 *  length of its edges.
 *
 *  The solution of a minimum cycle basis problem can be in the following three forms.
 *  - A pair (mcb, proof) where both are arrays of mcb::spvecgf2, array< mcb::spvecgf2 >.
 *    Each mcb::spvecgf2 represents a cycle or some edge set. Each edge of the graph
 *    is represented by a unique number provided by an edge numbering, mcb::edge_num.
 *  - A pair (mcb, proof) where both are arrays of compressed integer sets, array< d_int_set >.
 *      Each integer sets represents a cycle. In these integer sets, each edge of the graph 
 *      is represented by a unique number provided by the edge numbering, mcb::edge_num.
 *  - A pair (mcb, proof) where both are arrays of lists of edges, array< list<edge> >.
 *      Each element of this array represents a cycle of the minimum cycle basis.
 *
 *  Most functions of this section are templates functions. The template parameter W can be instantiated
 *  with any number type. Attention must be paid in order to avoid overflow of values.
 *
 *  The whole package is protected using a namespace called "mcb", and therefore using 
 *  anything requires <b>mcb::xxx</b> or the <b>using namespace mcb</b> directive.
 *
 *  \sa mcb::edge_num mcb::spvecgf2
 */

#ifndef MIN_CYCLE_BASIS_H
#define MIN_CYCLE_BASIS_H

#ifdef LEDA_GE_V5
#include <LEDA/graph/graph.h>
#include <LEDA/graph/edge_array.h>
#include <LEDA/graph/node_array.h>
#include <LEDA/graph/shortest_path.h>
#include <LEDA/core/d_int_set.h>
#include <LEDA/core/array.h>
#include <LEDA/core/list.h>
#include <LEDA/core/sortseq.h>
#include <LEDA/core/tuple.h>
#else
#include <LEDA/graph.h>
#include <LEDA/d_int_set.h>
#include <LEDA/edge_array.h>
#include <LEDA/node_array.h>
#include <LEDA/array.h>
#include <LEDA/list.h>
#include <LEDA/sortseq.h>
#include <LEDA/shortest_path.h>
#include <LEDA/tuple.h>
#endif

#include <LEP/mcb/SIGNED_GRAPH.h>

// start our namespace
namespace mcb 
{ 

#if defined(LEDA_NAMESPACE)
using leda::graph;
using leda::array;
using leda::edge;
using leda::edge_array;
using leda::d_int_set;
using leda::list;
using leda::sortseq;
using leda::three_tuple;
using leda::two_tuple;
using leda::DIJKSTRA_T;
using leda::node_set;
#endif




// external 
extern bool MCB_verify_basis( const leda::graph& g,
                              leda::array< leda::d_int_set >& mcb,
                              const mcb::edge_num& enumb,
                              bool check_cycle_basis 
                            );

extern void MCB_output_basis( const leda::graph& g,
                              leda::array< leda::d_int_set >& C,
                              const mcb::edge_num& enumb,
                              leda::array< leda::list< leda::edge > >& mcb );

extern void MCB_output_basis( const leda::graph& g, 
                              const leda::array< leda::list<leda::edge> >& mcb,
                              const mcb::edge_num& enumb,
                              leda::array< leda::d_int_set >& C );

extern void MCB_output_basis( const leda::graph& g,
                              const leda::array< mcb::spvecgf2 >& C,
                              const mcb::edge_num& enumb,
                              leda::array< leda::list< leda::edge > >& mcb );

extern void MCB_output_basis( const leda::graph& g,
                              const leda::array< mcb::spvecgf2 >& C,
                              const mcb::edge_num& enumb,
                              leda::array< leda::d_int_set >& mcb );

/*! \brief Compute a MCB of an undirected unweighted graph using de Pina's algorithm.
 * 
 *  The function computes a Minimum Cycle
 *  Basis \f$B\f$ of a graph \f$g\f$, that is a cycle basis of \f$g\f$ with the smallest 
 *  length (sum of lengths of cycles).
 *  The basis is returned as an array of dynamic integer sets, leda::d_int_set, called mcb.<br>
 *
 *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
 *  numbering is represented by enumb. The function also returns a certificate of optimality of 
 *  the minimum cycle basis. More precisely a set of linearly independent vectors for 
 *  which cycle \f$i\f$ is the shortest cycle in \f$g\f$ with odd intersection with the 
 *  proof vector \f$i\f$.<br>
 *
 *  The function returns the weight of the Minimum Cycle Basis or is undefined
 *  if there were any errors.<br>
 *  The running time is \f$O( m^3 )\f$ where \f$m\f$ are the number of edges of \f$g\f$. 
 *
 *  \param g An undirected graph.
 *  \param mcb A leda::array of leda::d_int_set to return the MCB.
 *  \param proof A leda::array of leda::d_int_set to return the proof.
 *  \param enumb An edge numbering.
 *  \return The length of the MCB or undefined if some error occured.
 *  \pre g is undirected, simple and loopfree.
 */
extern int MIN_CYCLE_BASIS_DEPINA( const leda::graph& g,
                            leda::array< leda::d_int_set >& mcb,
                            leda::array< leda::d_int_set >& proof,
                            const mcb::edge_num& enumb
                            );

/*! \brief Compute a MCB of an undirected unweighted graph using de Pina's algorithm.
 * 
 *  The function computes a Minimum Cycle
 *  Basis \f$B\f$ of a graph \f$g\f$, that is a cycle basis of \f$g\f$ with the smallest 
 *  length (sum of lengths of cycles).
 *  The basis is returned as an array of dynamic integer sets, leda::d_int_set, called mcb.<br>
 *
 *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
 *  numbering is represented by enumb.
 *
 *  The function returns the weight of the Minimum Cycle Basis or is undefined
 *  if there were any errors.<br>
 *  The running time is \f$O( m^3 )\f$ where \f$m\f$ are the number of edges of \f$g\f$. 
 *
 *  \param g An undirected graph.
 *  \param mcb A leda::array of leda::d_int_set to return the MCB.
 *  \param enumb An edge numbering.
 *  \return The length of the MCB or undefined if some error occured.
 *  \pre g is undirected, simple and loopfree.
 */
extern int MIN_CYCLE_BASIS_DEPINA( const leda::graph& g,
                            leda::array< leda::d_int_set >& mcb,
                            const edge_num& enumb
                            );

/*! \brief Compute a MCB of an undirected unweighted graph using de Pina's algorithm.
 * 
 *  The function computes a Minimum Cycle
 *  Basis \f$B\f$ of a graph \f$g\f$, that is a cycle basis of \f$g\f$ with the smallest 
 *  length (sum of lengths of cycles).
 *  The basis is returned as an array of sparse vectors, spvecgf2, called mcb.<br>
 *
 *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
 *  numbering is represented by enumb. The function also returns a certificate of optimality of 
 *  the minimum cycle basis. More precisely a set of linearly independent vectors for 
 *  which cycle \f$i\f$ is the shortest cycle in \f$g\f$ with odd intersection with the 
 *  proof vector \f$i\f$.<br>
 *
 *  The function returns the weight of the Minimum Cycle Basis or is undefined
 *  if there were any errors.<br>
 *  The running time is \f$O( m^3 )\f$ where \f$m\f$ are the number of edges of \f$g\f$. 
 *
 *  \param g An undirected graph.
 *  \param mcb A leda::array of spvecgf2 to return the MCB.
 *  \param proof A leda::array of spvecgf2 to return the proof.
 *  \param enumb An edge numbering.
 *  \return The length of the MCB or undefined if some error occured.
 *  \pre g is undirected, simple and loopfree.
 */
extern int MIN_CYCLE_BASIS_DEPINA( const leda::graph& g,
                            leda::array< mcb::spvecgf2 >& mcb,
                            leda::array< mcb::spvecgf2 >& proof,
                            const edge_num& enumb
                            );

/*! \brief Compute a MCB of an undirected unweighted graph using de Pina's algorithm.
 * 
 *  The function computes a Minimum Cycle
 *  Basis \f$B\f$ of a graph \f$g\f$, that is a cycle basis of \f$g\f$ with the smallest 
 *  length (sum of lengths of cycles).
 *  The basis is returned as an array of sparse vectors, spvecgf2, called mcb.<br>
 *
 *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
 *  numbering is represented by enumb.
 *
 *  The function returns the weight of the Minimum Cycle Basis or is undefined
 *  if there were any errors.<br>
 *  The running time is \f$O( m^3 )\f$ where \f$m\f$ are the number of edges of \f$g\f$. 
 *
 *  \param g An undirected graph.
 *  \param mcb A leda::array of spvecgf2 to return the MCB.
 *  \param enumb An edge numbering.
 *  \return The length of the MCB or undefined if some error occured.
 *  \pre g is undirected, simple and loopfree.
 */
extern int MIN_CYCLE_BASIS_DEPINA( const leda::graph& g,
                            leda::array< mcb::spvecgf2 >& mcb,
                            const edge_num& enumb
                            );

/*! \brief Compute a MCB of an undirected unweighted graph using de Pina's algorithm.
 * 
 *  The function computes a Minimum Cycle
 *  Basis \f$B\f$ of a graph \f$g\f$, that is a cycle basis of \f$g\f$ with the smallest 
 *  length (sum of lengths of cycles).
 *  The basis is returned as an array of list of edges called mcb.
 *  The function also returns a certificate of optimality of 
 *  the minimum cycle basis. More precisely a set of linearly independent vectors for 
 *  which cycle \f$i\f$ is the shortest cycle in \f$g\f$ with odd intersection with the 
 *  proof vector \f$i\f$.<br>
 *
 *  The function returns the weight of the Minimum Cycle Basis or is undefined
 *  if there were any errors.<br>
 *  The running time is \f$O( m^3 )\f$ where \f$m\f$ are the number of edges of \f$g\f$. 
 *
 *  \param g An undirected graph.
 *  \param mcb An array of list of edges to return the MCB.
 *  \param proof An array of list of edges to return the proof.
 *  \return The length of the MCB or undefined if some error occured.
 *  \pre g is undirected, simple and loopfree.
 */
extern int MIN_CYCLE_BASIS_DEPINA( const leda::graph& g,
                            leda::array< leda::list<leda::edge> >& mcb,
                            leda::array< leda::list<leda::edge> >& proof
                            );

/*! \brief Compute a MCB of an undirected unweighted graph using de Pina's algorithm.
 * 
 *  The function computes a Minimum Cycle
 *  Basis \f$B\f$ of a graph \f$g\f$, that is a cycle basis of \f$g\f$ with the smallest 
 *  length (sum of lengths of cycles).
 *  The basis is returned as an array of list of edges called mcb.
  *
 *  The function returns the weight of the Minimum Cycle Basis or is undefined
 *  if there were any errors.<br>
 *  The running time is \f$O( m^3 )\f$ where \f$m\f$ are the number of edges of \f$g\f$. 
 *
 *  \param g An undirected graph.
 *  \param mcb A array of list of edges to return the MCB.
 *  \return The length of the MCB or undefined if some error occured.
 *  \pre g is undirected, simple and loopfree.
 */
extern int MIN_CYCLE_BASIS_DEPINA( const leda::graph& g,
                            leda::array< leda::list<leda::edge> >& mcb
                            );

// unweighted hybrid algorithm
extern void MCB_construct_superset( const leda::graph& G,
                                    leda::sortseq< leda::two_tuple<int,int>, leda::d_int_set >& super, 
                                    const edge_num& enumb);

extern void MCB_construct_unweighted_paths( const leda::graph& G,
        leda::node_array< leda::node_array<leda::edge> >& pred );

/*! Compute a minimum cycle basis using a hybrid algorithm.
 *
 *  The function computes a minimum cycle basis of an undirected
 *  graph \f$g\f$, that is a cycle basis of \f$g\f$ with the smallest length 
 *  (sum of lengths of cycles). The 
 *  basis is returned as an array of dynamic integer sets, d_int_set, called mcb. 
 *
 *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
 *  numbering is represented by enumb. 
 *
 *  The function also returns a certificate of optimality of 
 *  the minimum cycle basis. More precisely a set of linearly independent vectors for 
 *  which cycle \f$i\f$ is the shortest cycle in \f$g\f$ with odd intersection with the 
 *  proof vector \f$i\f$. 
 *
 *  The function returns the weight of the Minimum Cycle Basis or is undefined
 *  if there were any errors.<br> 
 *  The running time is \f$O( m^2 n^2 )\f$ where \f$m\f$ are the number of edges of \f$g\f$ 
 *  and \f$n\f$ the number of vertices. 
 *  \param g An undirected graph.
 *  \param mcb A leda::array of leda::d_int_set to return the MCB.
 *  \param proof A leda::array of leda::d_int_set to return the proof.
 *  \param enumb An edge numbering.
 *  \return The length of the MCB or undefined if some error occured.
 *  \pre g is undirected, simple and loopfree 
 */
extern int MIN_CYCLE_BASIS_HYBRID( const leda::graph& g,
                                   leda::array< leda::d_int_set >& mcb,
                                   leda::array< leda::d_int_set >& proof,
                                   const edge_num& enumb
                                 );

/*! Compute a minimum cycle basis using a hybrid algorithm.
 *
 *  The function computes a minimum cycle basis of an undirected
 *  graph \f$g\f$, that is a cycle basis of \f$g\f$ with the smallest length 
 *  (sum of lengths of cycles). The 
 *  basis is returned as an array of dynamic integer sets, d_int_set, called mcb. 
 *
 *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
 *  numbering is represented by enumb. 
 *
 *  The function returns the weight of the Minimum Cycle Basis or is undefined
 *  if there were any errors.<br> 
 *  The running time is \f$O( m^2 n^2 )\f$ where \f$m\f$ are the number of edges of \f$g\f$ 
 *  and \f$n\f$ the number of vertices. 
 *  \param g An undirected graph.
 *  \param mcb A leda::array of leda::d_int_set to return the MCB.
 *  \param enumb An edge numbering.
 *  \return The length of the MCB or undefined if some error occured.
 *  \pre g is undirected, simple and loopfree 
 */
extern int MIN_CYCLE_BASIS_HYBRID( const leda::graph& g,
                                   leda::array< leda::d_int_set >& mcb,
                                   const edge_num& enumb
                                 );

/*! Compute a minimum cycle basis using a hybrid algorithm.
 *
 *  The function computes a minimum cycle basis of an undirected
 *  graph \f$g\f$, that is a cycle basis of \f$g\f$ with the smallest length 
 *  (sum of lengths of cycles). The 
 *  basis is returned as an array of lists of edges called mcb.
 *
 *  The function also returns a certificate of optimality of 
 *  the minimum cycle basis. More precisely a set of linearly independent vectors for 
 *  which cycle \f$i\f$ is the shortest cycle in \f$g\f$ with odd intersection with the 
 *  proof vector \f$i\f$. 
 *
 *  The function returns the weight of the Minimum Cycle Basis or is undefined
 *  if there were any errors.<br> 
 *  The running time is \f$O( m^2 n^2 )\f$ where \f$m\f$ are the number of edges of \f$g\f$ 
 *  and \f$n\f$ the number of vertices. 
 *  \param g An undirected graph.
 *  \param mcb A leda::array of leda::list of edges to return the MCB.
 *  \param proof A leda::array of leda::list of edges to return the proof.
 *  \return The length of the MCB or undefined if some error occured.
 *  \pre g is undirected, simple and loopfree 
 */
extern int MIN_CYCLE_BASIS_HYBRID( const leda::graph& g,
                                   leda::array< leda::list<leda::edge> >& mcb,
                                   leda::array< leda::list<leda::edge> >& proof
                                 );

/*! Compute a minimum cycle basis using a hybrid algorithm.
 *
 *  The function computes a minimum cycle basis of an undirected
 *  graph \f$g\f$, that is a cycle basis of \f$g\f$ with the smallest length 
 *  (sum of lengths of cycles). The 
 *  basis is returned as an array of lists of edges called mcb. 
 *
 *  The function returns the weight of the Minimum Cycle Basis or is undefined
 *  if there were any errors.<br> 
 *  The running time is \f$O( m^2 n^2 )\f$ where \f$m\f$ are the number of edges of \f$g\f$ 
 *  and \f$n\f$ the number of vertices. 
 *  \param g An undirected graph.
 *  \param mcb A leda::array of leda::list of edges to return the MCB.
 *  \return The length of the MCB or undefined if some error occured.
 *  \pre g is undirected, simple and loopfree 
 */
extern int MIN_CYCLE_BASIS_HYBRID( const leda::graph& g,
                                   leda::array< leda::list<leda::edge> >& mcb
                                   );

extern void swap( leda::d_int_set& a, leda::d_int_set& b );



template<class W, class CONT>
W MIN_CYCLE_BASIS_DEPINA( const graph& g,
                          const edge_array<W>& len,
                          array< CONT >& mcb,
                          array< CONT >& proof,
                          const edge_num& enumb
                          );

/*! \brief Compute a MCB of an undirected weighted graph using de Pina's algorithm.
 * 
 *  The function computes a Minimum Cycle
 *  Basis \f$B\f$ of a graph \f$g\f$, that is a cycle basis of \f$g\f$ with the smallest 
 *  length (sum of lengths of cycles).
 *  The basis is returned as an array of sparse vectors, spvecgf2, called mcb.<br>
 *
 *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
 *  numbering is represented by enumb. The function also returns a certificate of optimality of 
 *  the minimum cycle basis. More precisely a set of linearly independent vectors for 
 *  which cycle \f$i\f$ is the shortest cycle in \f$g\f$ with odd intersection with the 
 *  proof vector \f$i\f$.<br>
 *
 *  The function returns the weight of the Minimum Cycle Basis or is undefined
 *  if there were any errors.<br>
 *  The running time is \f$O( m^3 )\f$ where \f$m\f$ are the number of edges of \f$g\f$.
 *
 *  \param g An undirected graph.
 *  \param len A leda::edge_array for the edge lengths.
 *  \param mcb A leda::array of spvecgf2 to return the MCB.
 *  \param proof A leda::array of spvecgf2 to return the proof.
 *  \param enumb An edge numbering.
 *  \return The length of the MCB or undefined if some error occured.
 *  \pre g is undirected, simple and loopfree.
 *  \pre len is non-negative
 *  \remark Care must be taken when the template parameter is instantiated with a data type
 *          which has rounding errors.
 */
template<class W>
W MIN_CYCLE_BASIS_DEPINA( const graph& g,
                          const edge_array<W>& len,
                          array< spvecgf2 >& mcb,
                          array< spvecgf2 >& proof,
                          const edge_num& enumb
                          )
{
    return MIN_CYCLE_BASIS_DEPINA< W, spvecgf2 >( g,len,mcb,proof,enumb );
}

/*! \brief Compute a MCB of an undirected weighted graph using de Pina's algorithm.
 * 
 *  The function computes a Minimum Cycle
 *  Basis \f$B\f$ of a graph \f$g\f$, that is a cycle basis of \f$g\f$ with the smallest 
 *  length (sum of lengths of cycles).
 *  The basis is returned as an array of LEDA's dynamic integer sets, leda::d_int_set, called mcb.<br>
 *
 *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
 *  numbering is represented by enumb. The function also returns a certificate of optimality of 
 *  the minimum cycle basis. More precisely a set of linearly independent vectors for 
 *  which cycle \f$i\f$ is the shortest cycle in \f$g\f$ with odd intersection with the 
 *  proof vector \f$i\f$.<br>
 *
 *  The function returns the weight of the Minimum Cycle Basis or is undefined
 *  if there were any errors.<br>
 *  The running time is \f$O( m^3 )\f$ where \f$m\f$ are the number of edges of \f$g\f$.
 *
 *  \param g An undirected graph.
 *  \param len A leda::edge_array for the edge lengths.
 *  \param mcb A leda::array of leda::d_int_set to return the MCB.
 *  \param proof A leda::array of leda::d_int_set to return the proof.
 *  \param enumb An edge numbering.
 *  \return The length of the MCB or undefined if some error occured.
 *  \pre g is undirected, simple and loopfree.
 *  \pre len is non-negative
 *  \remark Care must be taken when the template parameter is instantiated with a data type
 *          which has rounding errors.
 */
template<class W>
W MIN_CYCLE_BASIS_DEPINA( const graph& g,
                          const edge_array<W>& len,
                          array< d_int_set >& mcb,
                          array< d_int_set >& proof,
                          const edge_num& enumb
                          )
{
    return MIN_CYCLE_BASIS_DEPINA< W, d_int_set >( g,len,mcb,proof,enumb );
}



template<class W, class CONT>
W MIN_CYCLE_BASIS_DEPINA( const graph& g,
                          const edge_array<W>& len,
                          array< CONT >& mcb,
                          array< CONT >& proof,
                          const edge_num& enumb
                          ) 
{ 
        #if ! defined(LEDA_CHECKING_OFF)
                if ( g.is_undirected() == false )
                        error_handler(999,"MIN_CYCLE_BASIS: illegal graph\
                                          (directed?)");
                if ( Is_Undirected_Simple( g ) == false )
                        error_handler(999,"MIN_CYCLE_BASIS: illegal graph\
                          (parallel,anti-parallel edges or loops?)");
        #endif

        
        #if ! defined(LEDA_CHECKING_OFF)
        edge e1;
        forall_edges( e1 , g ) { 
                if ( len[e1] < 0 ) 
                        error_handler(999,"MIN_CYCLE_BASIS: illegal edge \
                                          (negative weight)");
        }
        #endif

        int N = enumb.dim_cycle_space();
        mcb.resize( N ); // set size of destination array
        proof.resize(N);



#ifdef LEP_STATS
        float Tcycle = 0.0, Torthog = 0.0, Ttemp;
#endif

        array< CONT >& C = mcb;
        array< CONT >& S = proof;
        // initialize signed graph
        sgraph<W,leda::bin_heap> sg( g , len, enumb ) ; 


        // initialize cycles C and initial sets S's
        for( int i = 0 ; i < N; i++ ) { S[i].insert( i ); }
   
        W min = W(), ret = W();
        // start computing cycles
        for( int k = 0; k < N; k++ ) { 
                
                #ifdef LEP_STATS
                leda::used_time( Ttemp );
                #endif
                #ifndef MCB_LEP_UNDIR_NO_EXCHANGE_HEURISTIC
                // choose the sparsest witness from the basis
                int minS = k;
                for( int r = k+1; r < N; r++ ) { 
                    if ( S[r].size() < S[minS].size() )
                        minS = r;
                }
                if ( minS != k ) {  // swap
                    mcb::swap( S[k], S[minS] );
                }
                #endif

                #ifdef LEP_STATS
                Torthog += leda::used_time( Ttemp );
                #endif
                ret = sg.get_shortest_odd_cycle( S[k], C[k] ); 
                min+= ret;

                #ifdef LEP_STATS
                Tcycle += leda::used_time( Ttemp );
                #endif
                
                for( int l = k+1; l < N; l++ )   {
                    // cycles are smaller than sets, thus it is better
                    // to intersect C and S than S and C
                    if ( (C[k].intersect(S[l])).size() %2 == 1 )   {
                        S[ l ] %= S[k];
                    }
                }

                #ifdef LEP_STATS
                Torthog += leda::used_time( Ttemp );
                #endif


        }

#ifdef LEP_STATS
    std::cout << "LEP_STATS: cycle computation time: " << Tcycle << std::endl;
    std::cout << "LEP_STATS: orthogonal base maintain time: " << Torthog << std::endl;
#endif

        return min;
}


/*! \brief Compute a MCB of an undirected weighted graph using de Pina's algorithm.
 * 
 *  The function computes a Minimum Cycle
 *  Basis \f$B\f$ of a graph \f$g\f$, that is a cycle basis of \f$g\f$ with the smallest 
 *  length (sum of lengths of cycles).
 *  The basis is returned as an array of LEDA's dynamic integer sets, leda::d_int_set, called mcb.<br>
 *
 *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
 *  numbering is represented by enumb.
 *
 *  The function returns the weight of the Minimum Cycle Basis or is undefined
 *  if there were any errors.<br>
 *  The running time is \f$O( m^3 )\f$ where \f$m\f$ are the number of edges of \f$g\f$.
 *
 *  \param g An undirected graph.
 *  \param len A leda::edge_array for the edge lengths.
 *  \param mcb A leda::array of leda::d_int_set to return the MCB.
 *  \param enumb An edge numbering.
 *  \return The length of the MCB or undefined if some error occured.
 *  \pre g is undirected, simple and loopfree.
 *  \pre len is non-negative
 *  \remark Care must be taken when the template parameter is instantiated with a data type
 *          which has rounding errors.
 */
template<class W>
W MIN_CYCLE_BASIS_DEPINA( const graph& g,
                          const edge_array<W>& len,
                          array< d_int_set >& mcb,
                          const edge_num& enumb
                          ) 
{ 
    array< d_int_set > proof_temp;
    return MIN_CYCLE_BASIS_DEPINA( g, len, mcb, proof_temp, enumb );
}

/*! \brief Compute a MCB of an undirected weighted graph using de Pina's algorithm.
 * 
 *  The function computes a Minimum Cycle
 *  Basis \f$B\f$ of a graph \f$g\f$, that is a cycle basis of \f$g\f$ with the smallest 
 *  length (sum of lengths of cycles).
 *  The basis is returned as an array of spvecgf2, called mcb.<br>
 *
 *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
 *  numbering is represented by enumb.
 *
 *  The function returns the weight of the Minimum Cycle Basis or is undefined
 *  if there were any errors.<br>
 *  The running time is \f$O( m^3 )\f$ where \f$m\f$ are the number of edges of \f$g\f$.
 *
 *  \param g An undirected graph.
 *  \param len A leda::edge_array for the edge lengths.
 *  \param mcb A leda::array of spvecgf2 to return the MCB.
 *  \param enumb An edge numbering.
 *  \return The length of the MCB or undefined if some error occured.
 *  \pre g is undirected, simple and loopfree.
 *  \pre len is non-negative
 *  \remark Care must be taken when the template parameter is instantiated with a data type
 *          which has rounding errors.
 */
template<class W>
W MIN_CYCLE_BASIS_DEPINA( const graph& g,
                          const edge_array<W>& len,
                          array< spvecgf2 >& mcb,
                          const edge_num& enumb
                          ) 
{ 
    array< spvecgf2 > proof_temp;
    return MIN_CYCLE_BASIS_DEPINA( g, len, mcb, proof_temp, enumb );
}


/*! \brief Compute a MCB of an undirected weighted graph using de Pina's algorithm.
 * 
 *  The function computes a Minimum Cycle
 *  Basis \f$B\f$ of a graph \f$g\f$, that is a cycle basis of \f$g\f$ with the smallest 
 *  length (sum of lengths of cycles).
 *  The basis is returned as an array of lists of edges, called mcb.<br>
 *
 *  The function returns the weight of the Minimum Cycle Basis or is undefined
 *  if there were any errors.<br>
 *  The running time is \f$O( m^3 )\f$ where \f$m\f$ are the number of edges of \f$g\f$.
 *
 *  \param g An undirected graph.
 *  \param len A leda::edge_array for the edge lengths.
 *  \param mcb A leda::array of leda::list of edges to return the MCB.
 *  \return The length of the MCB or undefined if some error occured.
 *  \pre g is undirected, simple and loopfree.
 *  \pre len is non-negative
 *  \remark Care must be taken when the template parameter is instantiated with 
 *          a data type which has rounding errors.
 */
template<class W>
W MIN_CYCLE_BASIS_DEPINA( const graph& g, 
                          const edge_array<W>& len, 
                          array< list<edge> >& mcb )
{
        edge_num enumb(g);
        array< d_int_set > mcb_temp, proof_temp;
        W min = MIN_CYCLE_BASIS_DEPINA( g, len, mcb_temp, proof_temp, enumb );
        // reformat output
        MCB_output_basis( g, mcb_temp, enumb, mcb );
        return min;
}


/*! \brief Compute a MCB of an undirected weighted graph using de Pina's algorithm.
 * 
 *  The function computes a Minimum Cycle
 *  Basis \f$B\f$ of a graph \f$g\f$, that is a cycle basis of \f$g\f$ with the smallest 
 *  length (sum of lengths of cycles).
 *  The basis is returned as an array or lists of edges, called mcb.<br>
 *
 *  The function also returns a certificate of optimality of 
 *  the minimum cycle basis. More precisely a set of linearly independent vectors for 
 *  which cycle \f$i\f$ is the shortest cycle in \f$g\f$ with odd intersection with the 
 *  proof vector \f$i\f$.<br>
 *
 *  The function returns the weight of the Minimum Cycle Basis or is undefined
 *  if there were any errors.<br>
 *  The running time is \f$O( m^3 )\f$ where \f$m\f$ are the number of edges of \f$g\f$.
 *
 *  \param g An undirected graph.
 *  \param len A leda::edge_array for the edge lengths.
 *  \param mcb A leda::array of leda::list of edges to return the MCB.
 *  \param proof A leda::array of leda::list of edges to return the proof.
 *  \return The length of the MCB or undefined if some error occured.
 *  \pre g is undirected, simple and loopfree.
 *  \pre len is non-negative
 *  \remark Care must be taken when the template parameter is instantiated with a data type
 *          which has rounding errors.
 */
template<class W>
W MIN_CYCLE_BASIS_DEPINA( const graph& g, 
                          const edge_array<W>& len,
                          array< list<edge> >& mcb,
                          array< list<edge> >& proof) 
{
        edge_num enumb(g);
        array< d_int_set > mcb_temp, proof_temp;
        W min = MIN_CYCLE_BASIS_DEPINA( g, len, mcb_temp, proof_temp, enumb );
        // reformat output
        MCB_output_basis( g, mcb_temp, enumb, mcb );
        MCB_output_basis( g, proof_temp, enumb, proof );
        return min;
}


template<class W>
bool MIN_CYCLE_BASIS_DEPINA_CHECK( const graph& g,
                                   const edge_array<W>& len,
                                   array< d_int_set >& mcb,
                                   array< d_int_set >& proof,
                                   const edge_num& enumb ) 

/*\Mfunc $g$ is an undirected graph and $len$ is a non-negative length function on 
the edges of $g$. $mcb$ is a minimum cycle basis and $proof$ a certificate of 
optimality of $mcb$. \\
$enumb$ is the numbering on the edges of the graph which was used in order to 
construct the minimum cycle basis. \\
The function checks whether the $mcb$ is indeed a minimum cycle basis. \\
The running time is $O( m^3 )$ where $m$ are the number of edges of $g$. \\
\precond $G$ is undirected, simple and loopfree \\
\precond $len$ is non-negative \\
\precond $enumb$ is the same numbering as the one used to construct the $mcb$.
*/
{
        #if ! defined(LEDA_CHECKING_OFF)
                if ( g.is_undirected() == false )
                        error_handler(999,"MIN_CYCLE_BASIS: illegal graph\
                                          (directed?)");
                if ( Is_Undirected_Simple( g ) == false )
                        error_handler(999,"MIN_CYCLE_BASIS: illegal graph\
                          (parallel,anti-parallel edges or loops?)");
        #endif

        
        #if ! defined(LEDA_CHECKING_OFF)
        edge e1;
        forall_edges( e1 , g ) { 
                if ( len[e1] < 0 ) 
                        error_handler(999,"MIN_CYCLE_BASIS: illegal edge \
                                          (negative weight)");
        }
        #endif

        sgraph<W,leda::bin_heap> sg( g, len, enumb );

        if ( MCB_verify_basis( g, mcb, enumb, true ) == false ||
             MCB_verify_basis( g, proof, enumb, false ) == false ||
             mcb.size() != proof.size() ) 
                return false;

        // verify cycles
        W cycle_len;
        list<int> L;
        for( int i = 0; i < mcb.size(); i++ ) { 
                d_int_set TEMP;
                W min = sg.get_shortest_odd_cycle( proof[i] , TEMP );

                // compute cycle length
                cycle_len = W();
                L.clear();
                mcb[i].get_element_list(L);
                int el;
                forall( el, L ) cycle_len += len[ enumb(el) ];
        
                if ( cycle_len != min ) 
                        return false;
        }
        return true;
}

template<class W>
bool MIN_CYCLE_BASIS_DEPINA_CHECK( const graph& g,
                                   const edge_array<W>& len,
                                   array< spvecgf2 >& mcb,
                                   array< spvecgf2 >& proof,
                                   const edge_num& enumb ) 

{
#if ! defined(LEDA_CHECKING_OFF)
    if ( mcb.size() != proof.size() || mcb.size() != enumb.dim_cycle_space() ) 
        error_handler(999,"MIN_CYCLE_BASIS: wrong array sizes :(");
#endif
    array< d_int_set > mcbD;
    array< d_int_set > proofD;
    mcbD.resize( mcb.size() ); 
    proofD.resize( proof.size() );
    for( int i = 0; i < mcb.size(); i++ ) { 
        mcbD[i] = mcb[i].to_d_int_set();
        proofD[i] = proof[i].to_d_int_set();
    }
    return MIN_CYCLE_BASIS_DEPINA_CHECK( g, len, mcbD, proofD, enumb );
}



template<class W>
bool MIN_CYCLE_BASIS_DEPINA_CHECK( const graph& g,
                                   const edge_array<W>& len,
                                   array< list<edge> >& mcb,
                                   array< list<edge> >& proof 
                                   )
{
        edge_num enumb( g );
        array< d_int_set > mcb_temp, proof_temp;
        MCB_output_basis( g, mcb, enumb, mcb_temp );
        MCB_output_basis( g, proof, enumb, proof_temp );
        return MIN_CYCLE_BASIS_DEPINA_CHECK( g, len, mcb_temp, proof_temp, enumb );
}


// construct unique paths from v to u
template<class W>
void MCB_construct_paths( const graph& G,
        const edge_array<W>& len,
        node_array< node_array<edge> >& pred
        )
{
        // initialize
        node v;
        node_array<W> dist(G);
        
        // calculate paths
        forall_nodes( v, G ) {
                pred[v].init( G, nil );
        
                // run dijkstra
                DIJKSTRA_T<W>( G, v, len, dist, pred[v] );
        }
}

      
// construct Horton's superset of MCB
template<class W>
void MCB_construct_superset( const graph& G,
                const edge_array<W>& len,
                sortseq< three_tuple<W,int,int>, d_int_set >& super, 
                const edge_num& enumb)
{
    
    // initialize
    d_int_set C;
    node v,w,u,vz,ezs,ezt; 
    edge e,ez;

    int c = 0;
    node_array<int> degree(G,0);
    node_set touched( G );
    bool dege;

    W min;

    // construct paths
    node_array< node_array<edge> > pred( G );
    MCB_construct_paths<W>( G, len, pred );
        
    // loop - for all nodes
    forall_nodes( v , G ) { 

        // loop - for all edges
        forall_edges( e, G ) { 
            
            dege = false;

            w = G.source(e);
            u = G.target(e);

            // valid?
            if ( v == w || v == u || 
                 pred[ v ][ w ] == nil || pred[ v ][ u ] == nil ||
                 pred[ v ][ w ] == e || pred[ v ][ u ] == e || 
                 pred[ w ][ v ] == e || pred[ u ][ v ] == e ) 
                    continue;

            // try to construct cycle
            C.clear();

            // insert e
            C.insert( enumb(e) );
            ezs = G.source( e );
            ezt = G.target( e );
            degree[ ezs ]++;
            degree[ ezt ]++;
            touched.insert( ezs );
            touched.insert( ezt );



            min = len[ e ];

            // create two paths
            int en;
            node x,y; 

            // make sure path is unique
            if ( v->id() < w->id() ) { y = v; x = w; }
            else { y = w; x = v; }
            
            while( pred[ y ][ x ] != nil && dege == false ) {
                    ez = pred[ y ][ x ];
                    en = enumb( ez );
                    if ( ! C.member( en ) ) {
                            C.insert( en );
                            ezs = G.source( ez );
                            ezt = G.target( ez );
                            degree[ ezs ]++;
                            degree[ ezt ]++;
                            if ( degree[ ezs ] > 2 || degree[ ezt ] > 2 )
                                    dege = true;
                            touched.insert( ezs );
                            touched.insert( ezt );
                            min += len[ ez ];
                    }
                    x = G.opposite( ez, x );
            }
 
            if ( v->id() < u->id() ) { y = v; x = u; }
            else { y = u; x = v; }
            
            while( pred[ y ][ x ] != nil && dege == false ) {
                    ez = pred[ y ][ x ];
                    en = enumb( ez );
                    if ( ! C.member( en ) ) {
                            C.insert( en );
                            ezs = G.source( ez );
                            ezt = G.target( ez );
                            degree[ ezs ]++;
                            degree[ ezt ]++;
                            if ( degree[ ezs ] > 2 || degree[ ezt ] > 2 )
                                    dege = true;
                            touched.insert( ezs );
                            touched.insert( ezt );
                            min += len[ ez ];
                    }
                    x = G.opposite( ez, x );
            }
 
                        
            // init degree array, and check for degeneracy
            while( touched.empty() == false ) {
                vz = touched.choose();
                touched.del( vz );
                if ( degree[ vz ] == 1 ) dege = true;
                degree[ vz ] = 0;
            }

            if ( dege == true ) continue;

            // add cycle to superset
            three_tuple<W,int,int> tt( min, C.size(), c++ ); 
            super.insert( tt, C );
        }
    }
}


/*! Compute a minimum cycle basis using a hybrid algorithm.
 *
 *  The function computes a minimum cycle basis of an undirected weighted 
 *  graph \f$g\f$, that is a cycle basis of \f$g\f$ with the smallest length 
 *  (sum of lengths of cycles). The 
 *  basis is returned as an array of dynamic integer sets, d_int_set, called mcb. 
 *
 *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
 *  numbering is represented by enumb. 
 *
 *  The function also returns a certificate of optimality of 
 *  the minimum cycle basis. More precisely a set of linearly independent vectors for 
 *  which cycle \f$i\f$ is the shortest cycle in \f$g\f$ with odd intersection with the 
 *  proof vector \f$i\f$. 
 *
 *  The function returns the weight of the Minimum Cycle Basis or is undefined
 *  if there were any errors.<br> 
 *  The running time is \f$O( m^2 n^2 )\f$ where \f$m\f$ are the number of edges of \f$g\f$ 
 *  and \f$n\f$ the number of vertices. 
 *  \param g An undirected graph.
 *  \param len A leda::edge_array for the edge lengths.
 *  \param mcb A leda::array of leda::d_int_set to return the MCB.
 *  \param proof A leda::array of leda::d_int_set to return the proof.
 *  \param enumb An edge numbering.
 *  \return The length of the MCB or undefined if some error occured.
 *  \pre g is undirected, simple and loopfree 
 *  \pre len is non-negative 
 */
template<class W>
W MIN_CYCLE_BASIS_HYBRID( const graph& g,
                          const edge_array<W>& len,
                          array< d_int_set >& mcb,
                          array< d_int_set >& proof,
                          const edge_num& enumb
                          ) 
{ 
    #if ! defined(LEDA_CHECKING_OFF)
            if ( g.is_undirected() == false )
                    error_handler(999,"MIN_CYCLE_BASIS: illegal graph\
                                      (directed?)");
            if ( Is_Undirected_Simple( g ) == false )
                    error_handler(999,"MIN_CYCLE_BASIS: illegal graph\
                      (parallel,anti-parallel edges or loops?)");
    #endif

    
    #if ! defined(LEDA_CHECKING_OFF)
    edge e1;
    forall_edges( e1 , g ) { 
            if ( len[e1] < 0 ) 
                    error_handler(999,"MIN_CYCLE_BASIS: illegal edge \
                                      (negative weight)");
    }
    #endif

    int N = enumb.dim_cycle_space();
    mcb.resize( N ); // set size of destination array
    proof.resize(N);



    array< d_int_set >& C = mcb;
    array< d_int_set >& S = proof;
    
    // initialize cycles C and initial sets S's
    for( int i = 0 ; i < N; i++ ) { S[i].insert( i ); }

    // construct superset
    sortseq< three_tuple<W,int,int> ,d_int_set> super( leda::compare );
    MCB_construct_superset<W>( g, len, super, enumb );

#ifdef LEP_STATS
    static int lep_stats_queries = 0;
    std::cout << "LEP_STATS: horton set size: " << super.size() << std::endl;
#endif

    W min = W(), ret = W();
    leda::seq_item s;
    // start computing cycles
    for( int k = 0; k < N; k++ ) { 
        
        #ifdef LEP_STATS
        lep_stats_queries++;
        #endif

        // compute shortest odd cycle
        s = super.min_item();
        bool f = false;
        while ( s != nil ) { 
            if ( ( (super[s]).intersect( S[k] ) ).size() % 2 == 1 ) { // is odd
                C[k] = super[s];
                ret = super.key( s ).first();
                super.del_item( s );
                f = true; // found
                break;
            }
            s = super.succ( s );

        #ifdef LEP_STATS
            lep_stats_queries++;
        #endif
        }
        if ( f == false )
            error_handler(999,"MIN_CYCLE_BASIS (HYBRID):\
                    superset contains no MCB :(");
        min += ret;

        
        for( int l = k+1; l < N; l++ )   {
            // cycles are smaller than sets, thus it is better
            // to intersect C and S than S and C
            if ( (C[k].intersect(S[l])).size() %2 == 1 )   {
                S[ l ] %= S[k];
            }
        }

    }

#ifdef LEP_STATS
    std::cout << "LEP_STATS: horton set queries: " << lep_stats_queries << std::endl;
#endif

    return min;
}


/*! Compute a minimum cycle basis using a hybrid algorithm.
 *
 *  The function computes a minimum cycle basis of an undirected weighted 
 *  graph \f$g\f$, that is a cycle basis of \f$g\f$ with the smallest length 
 *  (sum of lengths of cycles). The 
 *  basis is returned as an array of dynamic integer sets, d_int_set, called mcb. 
 *
 *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
 *  numbering is represented by enumb. 
 *
 *  The function returns the weight of the Minimum Cycle Basis or is undefined
 *  if there were any errors.<br> 
 *  The running time is \f$O( m^2 n^2 )\f$ where \f$m\f$ are the number of edges of \f$g\f$ 
 *  and \f$n\f$ the number of vertices. 
 *  \param g An undirected graph.
 *  \param len A leda::edge_array for the edge lengths.
 *  \param mcb A leda::array of leda::d_int_set to return the MCB.
 *  \param enumb An edge numbering.
 *  \return The length of the MCB or undefined if some error occured.
 *  \pre g is undirected, simple and loopfree 
 *  \pre len is non-negative 
 */
template<class W>
W MIN_CYCLE_BASIS_HYBRID( const graph& g,
                          const edge_array<W>& len,
                          array< d_int_set >& mcb,
                          const edge_num& enumb
                          )
{
        array< d_int_set > proof_temp;
        return MIN_CYCLE_BASIS_HYBRID( g, len, mcb, proof_temp, enumb );
}

/*! Compute a minimum cycle basis using a hybrid algorithm.
 *
 *  The function computes a minimum cycle basis of an undirected weighted 
 *  graph \f$g\f$, that is a cycle basis of \f$g\f$ with the smallest length 
 *  (sum of lengths of cycles). The 
 *  basis is returned as an array of lists of edges called mcb. 
 *
 *  The function also returns a certificate of optimality of 
 *  the minimum cycle basis. More precisely a set of linearly independent vectors for 
 *  which cycle \f$i\f$ is the shortest cycle in \f$g\f$ with odd intersection with the 
 *  proof vector \f$i\f$. 
 *
 *  The function returns the weight of the Minimum Cycle Basis or is undefined
 *  if there were any errors.<br> 
 *  The running time is \f$O( m^2 n^2 )\f$ where \f$m\f$ are the number of edges of \f$g\f$ 
 *  and \f$n\f$ the number of vertices. 
 *  \param g An undirected graph.
 *  \param len A leda::edge_array for the edge lengths.
 *  \param mcb A leda::array of leda::list of edges to return the MCB.
 *  \param proof A leda::array of leda::list of edges to return the proof.
 *  \return The length of the MCB or undefined if some error occured.
 *  \pre g is undirected, simple and loopfree 
 *  \pre len is non-negative 
 */
template<class W>
W MIN_CYCLE_BASIS_HYBRID( const graph& g,
                          const edge_array<W>& len,
                          array< list<edge> >& mcb,
                          array< list<edge> >& proof
                          )
{
        array< d_int_set > mcb_temp, proof_temp;
        edge_num enumb( g );
        W min = MIN_CYCLE_BASIS_HYBRID( g, len, mcb_temp, proof_temp, enumb );
        MCB_output_basis( g, mcb_temp, enumb, mcb );
        MCB_output_basis( g, mcb_temp, enumb, proof );
        return min;
}

/*! Compute a minimum cycle basis using a hybrid algorithm.
 *
 *  The function computes a minimum cycle basis of an undirected weighted 
 *  graph \f$g\f$, that is a cycle basis of \f$g\f$ with the smallest length 
 *  (sum of lengths of cycles). The 
 *  basis is returned as an array of lists of edges called mcb. 
 *
 *  The function returns the weight of the Minimum Cycle Basis or is undefined
 *  if there were any errors.<br> 
 *  The running time is \f$O( m^2 n^2 )\f$ where \f$m\f$ are the number of edges of \f$g\f$ 
 *  and \f$n\f$ the number of vertices. 
 *  \param g An undirected graph.
 *  \param len A leda::edge_array for the edge lengths.
 *  \param mcb A leda::array of leda::d_int_set to return the MCB.
 *  \return The length of the MCB or undefined if some error occured.
 *  \pre g is undirected, simple and loopfree 
 *  \pre len is non-negative 
 */
template<class W>
W MIN_CYCLE_BASIS_HYBRID( const graph& g,
                          const edge_array<W>& len,
                          array< list<edge> >& mcb
                          )
{
        array< d_int_set > mcb_temp, proof_temp;
        edge_num enumb( g );
        W min = MIN_CYCLE_BASIS_HYBRID( g, len, mcb_temp, proof_temp, enumb );
        MCB_output_basis( g, mcb_temp, enumb, mcb );
        return min;
}


} // namespace mcb end

#endif  // MIN_CYCLE_BASIS_H

/* ex: set ts=8 sw=4 sts=4 noet: */


