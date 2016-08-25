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

/*! \file edge_num.h
 *  \brief Definition of edge numbering.
 */

#ifndef EDGE_NUM_H
#define EDGE_NUM_H

#include <LEP/mcb/config.h>
#include <vector>
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

namespace mcb { 


    /*! \brief An edge numbering class. 
     *
     * This class assigns a unique numbering to the edges of a graph. The \f$m\f$
     * graph edges are numbered from \f$0\f$ to \f$m-1\f$. The numbering is based on 
     * an arbitrary spanning tree \f$T\f$. Edges not in \f$T\f$ are numbered from 
     * \f$0\f$ to \f$m -n + \kappa -1\f$ where \f$\kappa\f$
     * are the number of (weakly) connected components of \f$G\f$. Edges in \f$T\f$ are numbered
     * from \f$m-n+\kappa\f$ to \f$m-1\f$.
     *
     * An edge numbering is implemented as two arrays, and therefore requires
     * \f$O(m)\f$ space. All operations take constant time except construction
     * which takes linear time.
     *
     * This is a static data structure. Changes in the graph after initializing an edge
     * numbering invalidate the data structure.
     *
     * \date 2004-2005
     * \author Dimitrios Michail
     */
    class edge_num
    {

        private:
            // variables:
            int n; 
            int m;
            int k;
            std::vector< leda::edge > index;
            leda::edge_array<int> rindex;
            // methods:
            void create_numbering( const leda::graph& );
            int construct_tree( const leda::graph&, leda::edge_array<bool>& tree );
            
        public:
            /*! Construct an edge numbering for the empty graph. */
            edge_num();

            /*! Construct an edge numbering for a graph.
             *  \param G The graph to construct for.
             */
            explicit edge_num ( const leda::graph& G );

            /*! Copy constructor */
            edge_num ( const edge_num& );

            /*! Destructor */
            ~edge_num (void);

            /*! Assignment operator */
            edge_num& operator=( const edge_num& );

            /*! Access the number of an edge.
             *  \param e The edge to access.
             *  \return The unique number of the edge.
             */
            inline int operator()(leda::edge e) const { 
                return rindex[e];
            }

            /*! Access the edge with a particular number.
             *  \param i An integer from \f$0\f$ to \f$m-1\f$.
             *  \return The edge corresponding to that integer.
             */
            inline leda::edge operator()(int i) const { 
#if ! defined(LEDA_CHECKING_OFF)
                if ( i < 0 || i > m-1 )
                    leda::error_handler(999,"edge_num: illegal number requested");
#endif
                return index[ i ];
            }

            /*! Check if an edge belongs to the spanning forest used to construct the numbering.
             *  \param e An edge.
             *  \return True if e belongs to the spanning forest, false otherwise.
             */
            bool tree( leda::edge e ) const { 
                return ( rindex[e] >= m - n + k );
            }

            /*! Get the dimension of the cycle space of \f$G\f$. More precisely
             *  \f$m - n + \kappa\f$, where \f$\kappa\f$ is the number of (weakly)
             *  connected components of the graph.
             *  \return The dimension of the cycle space of the graph.
             */
            int dim_cycle_space() const {
                return m - n + k;
            }

            /*! Returns the number of (weakly) connected components of the graph. 
             *  \return The number of (weakly) connected components of the graph.
             */
            int num_weak_connected_comp() const {
                return k;
            }
    };


} // end namespace mcb

#endif

/* ex: set ts=4 sw=4 sts=4 et: */


