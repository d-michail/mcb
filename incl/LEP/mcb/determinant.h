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

/*! \file determinant.h
 *  \brief Compute the determinant of a cycle basis.
 */

#ifndef DETERMINANT_H
#define DETERMINANT_H

#include <LEP/mcb/spvecfp.h>
#include <LEP/mcb/edge_num.h>
#include <LEP/mcb/util.h>

#ifdef LEDA_GE_V5
#include <LEDA/graph/graph.h>
#include <LEDA/numbers/integer.h>
#include <LEDA/numbers/integer_matrix.h>
#else
#include <LEDA/graph.h>
#include <LEDA/integer.h>
#include <LEDA/integer_matrix.h>
#endif

// start our namespace
namespace mcb 
{ 

#if defined(LEDA_NAMESPACE)
    using leda::graph;
    using leda::array;
    using leda::edge;
    using leda::node;
    using leda::integer_matrix;
#endif
    
    //@{
    
    /*!  \brief Compute the cycle matrix of a cycle basis. 
     *
     *   Let \f$N\f$ be the dimension of the cycle space. The cycle matrix
     *   is an \f$N \times N\f$ matrix. Each row corresponds to a cycle in the
     *   cycle basis. Each column corresponds to an edge of the graph among
     *   the edges that have numbering in mcb::edge_num from 0 to N-1. The remaining
     *   edges are the edges of a fixed spanning tree.
     *
     *   The edges have values either -1,0,1 based on an arbitrary orientation of
     *   traversing the cycle and based on the orientation of the edges of the graph.
     *
     *   The absolute value of the determinant of this matrix describes the cycle
     *   basis. If it is positive then the cycle basis is a directed cycle basis.
     *   If it is an odd numbers then the basis is an undirected cycle basis. If
     *   it is 1 then the basis is an integral cycle basis.
     *
     *   \param g The graph
     *   \param cb The cycle basis
     *   \param enumb An edge numbering
     *   \param B The matrix to output the cycle matrix.
     *   \pre B has dimension \f$N \times N\f$.
     *   \pre \f$N>0\f$
     *   \ingroup mcbutil
     */ 
    template<class Container>
    void cycle_matrix ( const graph& g,
		                const array<Container>& cb,
		                const mcb::edge_num& enumb, 
                        integer_matrix& B ) 
    {
        int N = enumb.dim_cycle_space();
        if ( N == 0 )
            leda::error_handler(999,"determinant: cycle space dimension is zero!");

        if ( B.dim1() != N || B.dim2() != N )
            leda::error_handler(999,"determinant: matrix has wrong dimensions!");

        array< mcb::spvecfp > oriented_cb ( N );
        CycleOrienter<Container> orienter( g, enumb );
        for( int i = 0; i < N; ++i ) 
            oriented_cb[i] = orienter( cb[i] );
        
        cycle_matrix<mcb::spvecfp>( g, oriented_cb, enumb, B );
    }

    template<>
    void cycle_matrix<mcb::spvecfp> ( const graph& g,
		                const array<mcb::spvecfp>& cb,
		                const mcb::edge_num& enumb, 
                        integer_matrix& B ) 
    {
        int N = enumb.dim_cycle_space();
        if ( N == 0 )
            leda::error_handler(999,"cycle_matrix: cycle space dimension is zero!");

        if ( B.dim1() != N || B.dim2() != N )
            leda::error_handler(999,"cycle_matrix: matrix must have dimension NxN!");

        for( int i = 0; i < N; ++i ) {
            leda::list_item it = cb[i].first();
            while(it!=nil)
            {
                int index = cb[i].index(it);
                if( ! enumb.tree( enumb( index ) ) )
                {
                    B[i][index] = cb[i].inf(it);
                }
                it = cb[i].succ(it);
            }
        }
    }


    
    /*! \brief Output a LEDA integer_matrix in a format compatible with maple. 
     *  \param out The output stream
     *  \param B The matrix to output
     *  \return The output stream after outputing.
     *  \ingroup mcbutil
     */ 
    std::ostream& output_maple_format( std::ostream& out, const integer_matrix& B );


    /*! \brief Compute the determinant of a cycle basis.
     * 
     *  \param g A graph.
     *  \param cb A leda::array of Container with a cycle basis. Container can be
     *             either mcb::spvecgf2 and leda::d_int_set for undirected graphs
     *             or mcb::spvecfp for directed.
     *  \param enumb An edge numbering.
     *  \return The determinant of the cycle basis, as a leda::integer.
     *  \pre g is loopfree.
     *  \ingroup mcbutil
     */
    template<class Container>
	leda::integer determinant ( const graph& g,
                                const array< Container >& cb,
                                const mcb::edge_num& enumb
                              ) 
	{
        int N = enumb.dim_cycle_space();
        if ( N == 0 )
            leda::error_handler(999,"determinant: cycle space dimension is zero!");

        integer_matrix B( N, N ); 
        cycle_matrix<Container>( g, cb, enumb, B );

        leda::integer d = abs( determinant(B) );
        if ( d == 0 )
            leda::error_handler(999,"determinant: not a directed cycle basis!");
        return d;
	}


    //@}

} // namespace mcb end

#endif  // DETERMINANT_H

/* ex: set ts=4 sw=4 sts=4 et: */


