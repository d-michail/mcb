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

/*! \file dmcb.h
 *  \brief Algorithms for directed MCB.
 *
 *  Given an directed graph \f$G(V,E)\f$ and a positive length function on the edges
 *  \f$w: E \mapsto \mathcal{R}_{> 0}\f$, a minimum cycle basis is a set of cycles
 *  which can generate the cycle space and at the same time has minimum total length.
 *
 *  Each cycle of the graph is assumed to be a vector in \f$Q^m\f$ indexed on the edge set, and
 *  operations between cycles is performed in \f$Q\f$. The length of a cycle basis is 
 *  the sum of the length of its cycles and the length of a cycle is the sum of the 
 *  length of its edges.
 *
 *  Most functions of this section are templates functions. The template parameter W denoting the 
 *  type of the edge weights can be instantiated with any number type. Attention must be paid in order 
 *  to avoid overflow of values.

 *  The solution of a minimum cycle basis problem can be in the following two forms.
 *  - A pair (mcb, proof) where both are arrays of mcb::spvecfp. A mcb::spvecfp is a 
 *        wrapper around Leda's list datatype. Each element in this list, is a 
 *        two_tuple<long,integer>, where the first argument is its index \f$(0 \dots m-1)\f$ and
 *        the second is in case of mcb a value of  \f${\pm}1\f$ where positive is an arbitrary 
 *        direction of traversing the cycle and in case of proof a value in \f$Z_p\f$ for 
 *        some prime number \f$p\f$. 
 *        Each index represents an edge.<br> 
 *        The number of an edge can be found by the edge numbering, edge_num.
 *  - A solution mcb which is an array of arrays of mcb::etype (currently short ints), 
 *        array<mcb::etype>.
 *        Each element of this array represents a cycle of the minimum cycle basis.
 *        Each entry \f$i\f$ of the array is \f$0\f$ or \f${\pm}1\f$, based on whether the edge \f$e\f$ with 
 *        number \f$i\f$ ( enumb(e) = i ) belongs to the cycle and if yes in which direction
 *        compared with an arbitrary direction of traversing the cycle.
 *
 *  The whole package is protected using a namespace called "mcb", and therefore using 
 *  anything requires <b>mcb::xx</b> or the <b>using namespace mcb</b> directive.
 * 
 *  \sa mcb::spvecfp
 */

#ifndef DMCB_H
#define DMCB_H

#include <LEP/mcb/config.h>


#ifdef LEDA_GE_V5
#include <LEDA/graph/graph.h>
#include <LEDA/graph/edge_array.h>
#include <LEDA/core/array.h>
#include <LEDA/core/random_source.h>
#include <LEDA/numbers/integer.h>
#else
#include <LEDA/graph.h>
#include <LEDA/array.h>
#include <LEDA/edge_array.h>
#include <LEDA/integer.h>
#include <LEDA/random_source.h>
#endif

#include <LEP/mcb/edge_num.h>
#include <LEP/mcb/fp.h>
#include <LEP/mcb/spvecfp.h>
#include <LEP/mcb/arithm.h>
#include <LEP/mcb/transform.h>
#include <LEP/mcb/dsigned.h>

namespace mcb { 

#if defined(LEDA_NAMESPACE)
    using leda::graph;
    using leda::edge;
    using leda::edge_array;
    using leda::node;
    using leda::node_array;
    using leda::array;
    using leda::integer;
    using leda::random_source;
    using leda::pq_item;
#endif

    /*! \name Directed Minimum Cycle Basis
    */

    //@{
    // W = weight type
    // ptype = type of prime p

    /*  \brief Compute a cycle basis of a weighted directed graph modulo a prime number.
     * 
     *  The function computes a directed Cycle Basis \f$B\f$ of \f$g\f$, that 
     *  is a cycle basis of \f$g\f$ with the smallest length (sum of lengths of cycles). The 
     *  basis is returned as an array of sparse vectors spvecfp, called mcb. 
     * 
     *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
     *  numbering is represented by enumb. 
     * 
     *  The function also returns a certificate of optimality of the minimum cycle basis. 
     *  More precisely a set of linearly independent vectors for which cycle \f$i\f$ is the 
     *  shortest cycle in \f$g\f$ with non-zero intersection with the proof vector \f$i\f$. 
     * 
     *  The returned cycle basis might not be a minimum cycle basis.
     *  
     *  The function returns the weight of the cycle basis or is undefined
     *  if there were any errors. 
     *
     *  The running time is \f$O( m^3 )\f$ where \f$m\f$ are the number of edges of \f$g\f$. 
     *
     *  \param g A directed graph.
     *  \param len The edge lengths.
     *  \param mcb A leda::array of spvecfp to return the cycle basis.
     *  \param proof A leda::array of spvecfp to return the proof.
     *  \param enumb An edge numbering.
     *  \param p A leda::integer prime number.
     *  \pre g is simple and loop-free
     *  \pre len is positive
     *  \pre enumb is already initialized with g
     *  \pre p is a prime number
     *  \ingroup exactmcb
     */
    template<class W>
        W DMCB( const graph& g, 
                const edge_array<W>& len,
                array< mcb::spvecfp >& mcb,
                array< mcb::spvecfp >& proof,
                const mcb::edge_num& enumb,
                ptype& p
              )
        { 
#if ! defined(LEDA_CHECKING_OFF)
            if ( Is_Simple( g ) == false ) 
                error_handler(999,"DMCB: illegal graph (non-simple?)");
            if ( Is_Loopfree( g ) == false ) 
                error_handler(999,"DMCB: illegal graph (has loops?)");

            edge e1;
            forall_edges( e1 , g ) {
                if ( len[e1] <= 0 )
                    error_handler(999,"MIN_CYCLE_BASIS: illegal edge (non-positive weight)");
            }

            if ( ! primes<ptype>::is_prime( p ) ) 
                error_handler(999,"DMCB: p is not a prime number!");

#endif
            int d = enumb.dim_cycle_space();
            if ( d <= 0 ) return W(0);

            mcb.resize( d );
            array< spvecfp >& B = mcb;
            proof.resize( d );
            array< spvecfp >& X = proof;

            // initialize shortest paths
            dirsp<W,ptype> SP( g, len, p, enumb );

#if  defined(LEP_DEBUG_OUTPUT)
            std::cout << "executing with prime p = " << p << std::endl;
#endif

            // initialize X_i's and $B_i$'s
            // assume that $p$ fits in ptype
            // and $d$ in indextype
            indextype i,j;
            for( i = 0; i < d; i++ ) { 
                X[i] = spvecfp( p );
                X[i] = i;
                B[i] = spvecfp( p );
            }

            // now execute main loop
            spvecfp tmp = spvecfp( p );
            ptype tmpi;
            W min = W(0);
            for( i = 0; i < d; i++ ) { 

                // compute B_i
                W mini;
                B[i] = SP.get_shortest_cycle( X[i], mini );
                min += mini;

                // precompute part
                // NOTE: we do not precompute inverses, since we don't want
                //       to have a dependency on the maximum size of an 
                //       array that will store these values
                //       p is O(d^2 logd) and thus O(logd) to compute inverse
                //       at most d times, thus O(d logd) = O(m logm) in total
                tmpi = X[i]*B[i];
                while( tmpi < 0 ) tmpi += p; // make [-i]_p = [p-i]_p
                while( tmpi >= p ) tmpi -= p; // make [i+p]_p = [i]_p
                tmp = X[i] * fp<ptype>::get_mult_inverse( tmpi, p );

                // update sets X_j, j > i
                for( j = i+1; j < d; j++ ) 
                    X[j] -=  tmp * (B[i] * X[j]) ;
            }

            return min;            

        } // end of DMCB

    /*! \brief Compute a minimum cycle basis of a weighted directed graph.
     * 
     *  The function computes a directed Minimum Cycle Basis \f$B\f$ of \f$g\f$, that 
     *  is a cycle basis of \f$g\f$ with the smallest length (sum of lengths of cycles). The 
     *  basis is returned as an array of sparse vectors spvecfp, called mcb. 
     * 
     *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
     *  numbering is represented by enumb. 
     * 
     *  The function also returns a certificate of optimality of the minimum cycle basis. 
     *  More precisely a set of linearly independent vectors for which cycle \f$i\f$ is the 
     *  shortest cycle in \f$g\f$ with non-zero intersection with the proof vector \f$i\f$. 
     * 
     *  Since the algorithm is a randomized Monte-Carlo algorithm, the error argument 
     *  which should be less that 1 represents the acceptable error probability that the
     *  returned cycle basis is not a minimum cycle basis.
     *  
     *  The function returns the weight of the Minimum Cycle Basis or is undefined
     *  if there were any errors. 
     *
     *  The running time is \f$O( m^3 )\f$ where \f$m\f$ are the number of edges of \f$g\f$. 
     *
     *  \param g A directed graph.
     *  \param len The edge lengths.
     *  \param mcb A leda::array of spvecfp to return the MCB.
     *  \param proof A leda::array of spvecfp to return the proof.
     *  \param enumb An edge numbering.
     *  \param error The error probability.
     *  \pre g is simple and loop-free
     *  \pre len is positive
     *  \pre enumb is already initialized with g
     *  \pre error is positive and less than one
     *  \ingroup exactmcb
     */
    template<class W>
        W DMCB( const graph& g, 
                const edge_array<W>& len,
                array< mcb::spvecfp >& mcb,
                array< mcb::spvecfp >& proof,
                const mcb::edge_num& enumb,
                double error = 0.375
              )
        { 
#if ! defined(LEDA_CHECKING_OFF)
            if ( Is_Simple( g ) == false ) 
                error_handler(999,"MIN_CYCLE_BASIS: illegal graph (non-simple?)");
            if ( Is_Loopfree( g ) == false ) 
                error_handler(999,"MIN_CYCLE_BASIS: illegal graph (has loops?)");
            if ( error <= 0 || error >= 1 ) 
                error_handler(999,"MIN_CYCLE_BASIS: error probability is out of range");

            edge e1;
            forall_edges( e1 , g ) {
                if ( len[e1] <= 0 )
                    error_handler(999,"MIN_CYCLE_BASIS: illegal edge (non-positive weight)");
            }
#endif
            int d = enumb.dim_cycle_space();
            if ( d <= 0 ) return W(0);

            mcb.resize( d );
            proof.resize( d );

            // decide how many times to execute the algorithm ( perror <= 3/8 = 0.375 )
            int times = (int) ceil(  log(error)/log(0.375) );

#if  defined(LEP_DEBUG_OUTPUT)
            std::cout << "Executing " << times; 
            std::cout << " number of times to achieve error probability ";
            std::cout << error << std::endl;
#endif

            // create X and B matrices
            array< spvecfp > X ( d );
            array< spvecfp > B ( d );
            W min_so_far = W(0);
            bool min_so_far_inf = true;

            // loop necessary times, for error probability to be correct
            while( times-- > 0 ) { 

                // pick random prime
                ptype p;
                {
                    int logd = log( integer( d + 1 ) );
                    int loglogd = log( integer( logd + 1 ) );
                    int randbits = 7 + 2 * logd + loglogd;
                    int failsafe = 50 * randbits;
                    int count = 0;

                    while( true ) { 
                        // loop failsafe, increase random bits
                        if ( count++ > failsafe ) { 
                            randbits++;
                            failsafe += 50;
                            count = 0;
                        }

                        // get random number
                        p = ptype::random( randbits );
                        p += d*d;
                        //std::cout << "testing p = " << p << std::endl;

                        // if is > 1 and prime break
                        if ( p > 1 && primes<ptype>::is_prime( p ) ) break;
                    }
                }

#if  defined(LEP_DEBUG_OUTPUT)
                std::cout << "executing with prime p = " << p << std::endl;
#endif

                W min = DMCB( g, len, B, X, enumb, p );

                // if found better, update
                if ( ( min_so_far_inf == true ) || 
                        ( min_so_far_inf == false && min < min_so_far ) ) { 
#if  defined(LEP_DEBUG_OUTPUT)
                    if ( min_so_far_inf == false )
                        std::cout << "found better solution with weight " << min << std::endl;
#endif
                    mcb = B;
                    proof = X;
                    min_so_far_inf = false;
                    min_so_far = min;
                }

            }
            return min_so_far;
        } // end of DMCB

    /*! \brief Compute a minimum cycle basis of a weighted directed graph.
     * 
     *  The function computes a directed Minimum Cycle Basis \f$B\f$ of \f$g\f$, that 
     *  is a cycle basis of \f$g\f$ with the smallest length (sum of lengths of cycles). The 
     *  basis is returned as an array of sparse vectors spvecfp, called mcb. 
     * 
     *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
     *  numbering is represented by enumb. 
     * 
     *  Since the algorithm is a randomized Monte-Carlo algorithm, the error argument 
     *  which should be less that 1 represents the acceptable error probability that the
     *  returned cycle basis is not a minimum cycle basis.
     *  
     *  The function returns the weight of the Minimum Cycle Basis or is undefined
     *  if there were any errors. 
     *
     *  The running time is \f$O( m^3 )\f$ where \f$m\f$ are the number of edges of \f$g\f$. 
     *
     *  \param g A directed graph.
     *  \param len The edge lengths.
     *  \param mcb A leda::array of spvecfp to return the MCB.
     *  \param enumb An edge numbering.
     *  \param error The error probability.
     *  \pre g is simple and loop-free
     *  \pre len is positive
     *  \pre enumb is already initialized with g
     *  \pre error is positive and less than one
     *  \ingroup exactmcb
     */
    template<class W>
        W DMCB( const graph& g, 
                const edge_array<W>& len,
                array< mcb::spvecfp >& mcb,
                const mcb::edge_num& enumb,
                double error = 0.375
              )
        {
            array< spvecfp > proof_tmp;
            return DMCB(g, len, mcb, proof_tmp, enumb, error ); 
        }

    /*! \brief Compute a minimum cycle basis of a weighted directed graph.
     * 
     *  The function computes a directed Minimum Cycle Basis \f$B\f$ of \f$g\f$, that 
     *  is a cycle basis of \f$g\f$ with the smallest length (sum of lengths of cycles). The 
     *  basis is returned as an array of arrays of integers. Each such array if indexed on 
     *  \f$1 \dots m\f$ and its entry can be \f$0\f$ or \f$\pm 1\f$. Which edge corresponds to
     *  which index in this array can be found by the edge numbering, enumb. Note that the
     *  edge numbering using an indexing from \f$0\f$ to \f$m-1\f$.
     * 
     *  Since the algorithm is a randomized Monte-Carlo algorithm, the error argument 
     *  which should be less that 1 represents the acceptable error probability that the
     *  returned cycle basis is not a minimum cycle basis.
     *  
     *  The function returns the weight of the Minimum Cycle Basis or is undefined
     *  if there were any errors. 
     *
     *  The running time is \f$O( m^3 )\f$ where \f$m\f$ are the number of edges of \f$g\f$. 
     *
     *  \param g A directed graph.
     *  \param len The edge lengths.
     *  \param mcb A leda::array of leda::arrays of ints to return the MCB.
     *  \param enumb An edge numbering.
     *  \param error The error probability.
     *  \pre g is simple and loop-free
     *  \pre len is positive
     *  \pre enumb is already initialized with g
     *  \pre error is positive and less than one
     *  \ingroup exactmcb
     */
    template<class W>
        W DMCB( const graph& g, 
                const edge_array<W>& len,
                array< array<etype> >& mcb,
                const mcb::edge_num& enumb,
                double error = 0.375
              )
        {
            array< spvecfp > mcb_tmp;
            array< spvecfp > proof_tmp;

            int d = enumb.dim_cycle_space();

            // run the general version
            W min = DMCB<W>( g, len, mcb_tmp, \
                    proof_tmp, enumb, error );

            // get p used
            ptype p = proof_tmp[0].pvalue(); 

            // transform
            mcb.resize( d );
            for ( int i = 0; i < d; i++ )
                spvecfp_to_array_ints( g, enumb, p, mcb_tmp[i], mcb[i] );       

            return min;
        }

    //@}

} // end of mcb namespace

#endif // DMCB_H

/* ex: set ts=4 sw=4 sts=4 et: */
