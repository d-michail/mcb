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


/*! \file spvecfp.h
 *  \brief Sparse vector in \f$F_p\f$.
 */

#ifndef SPVECFP_H
#define SPVECFP_H

#include <LEP/mcb/config.h>
#include <iostream>

#ifdef LEDA_GE_V5
#include <LEDA/core/array.h>
#include <LEDA/core/list.h>
#include <LEDA/core/tuple.h>
#include <LEDA/system/error.h>
#include <LEDA/system/assert.h>
#else
#include <LEDA/array.h>
#include <LEDA/list.h>
#include <LEDA/tuple.h>
#include <LEDA/error.h>
#include <LEDA/assert.h>
#endif

#include <LEP/mcb/edge_num.h>
#include <LEP/mcb/arithm.h>

namespace mcb { 

#if defined(LEDA_NAMESPACE)
    using leda::list;
    using leda::list_item;
    using leda::error_handler;
    using leda::two_tuple;
    using leda::three_tuple;
    using leda::array;
#endif

    
    /*! \brief A sparse vector with elements in \f$F_p\f$.
     *
     *  This class implements a sparse vector with elements in \f$F_p\f$. The supported operations are
     *  limited to those required by the cycle basis algorithms.
     *
     *  The internal representation is a list of tuples, one for each non-zero entry of the sparse vector. 
     *  Each tuple contains two integers, the index of the non-zero entry and its value. This entries are
     *  supposed to be sorted in order for the various binary operators to work properly. The function
     *  append does not ensure this, it is up to the user to make sure that the correct order is maintained.
     *
     *  \remark Indices are between \f$0\f$ and \f$len-1\f$ where \f$len\f$ is the length of the vector.
     *
     *  \date 2005
     *  \author Dimitrios Michail
     */
    class spvecfp { 
        
        public:

        /*! Default Constructor */
        spvecfp();
        /*! Constructor
         *  \param p Prime number.
         */
        spvecfp( const ptype& p );
        
        /*! Copy constructor */
        spvecfp( const spvecfp& a );

        /*! Descructor */
        ~spvecfp();

        /*! Clear the vector and reinitialize it.
         *  \param p Prime number.
         */
        void reset( const ptype& p );

        // assign a vector to the current vector
        // cast between types, retain the current's vector type
        spvecfp& operator=( const spvecfp& i );

        // assign current vector to e_i
        spvecfp& operator=( const indextype& i );

        /*! Negate the current sparse vector.
         *  \return A new vector corresponding to current vector negated. 
         */
        spvecfp operator-() const;

        /*! Compute the inner product of two vectors.
         *  \param a A sparse vector.
         *  \return The inner product of this vector and a.
         */
        ptype operator*( const spvecfp& a ) const;

        /*! Add two vectors.
         *  \param a A sparse vector.
         *  \return The sum of this vector and a.
         */
        spvecfp operator+( const spvecfp& a ) const;

        /*! Compute the product with a constant.
         *  \param a A constant.
         *  \return The product of the current vector with a constant.
         */
        spvecfp operator*( const ptype& a );

        /*! Add a vector to the current vector.
         *  \param a A sparse vector a.
         *  \return The current vector after adding a.
         */
        spvecfp& operator+=( const spvecfp& a );

        /*! Subtract a vector from the current vector.
         *  \param a A sparse vector a.
         *  \return The current vector after subtracting a.
         */
        spvecfp& operator-=( const spvecfp& a );

        /*! Print the vector to a stream. 
         *  \param o The stream to print at.
         */
        void print( std::ostream& o ) const;

        /*! Append an entry to the sparse vector. The internal representation 
         *  is a list of sorted entries by index. This procedure does not enforce
         *  this order, it simply appends the new entry. Use wisely.
         *  \param index Index of the new element to append.
         *  \param value The value of the new element to append.
         *  \remark No attempt to preserve correct sorted order is done. All elements
         *          in the vector must have index values less that the new element's
         *          for the resulting sparse vector to be valid.
         */
        void append( indextype index, const ptype& value );

        /*! Sort the internal representation of the sparse vector. This operation
         *  ensures a correct representation after its call. Should not be
         *  necessary unless append was called in wrong order.
         */
        void sort();

        // define access stuff, like leda lists

        /*! Check if the vector is empty, all elements are zero.
         *  \return True if empty, false otherwise.
         */
        bool empty() const;

        /*! Make all elements zero. */
        void clear();

        /*! Get the number of non-zero entries.
         *  \return The number of non-zero entries in the vector.
         */
        indextype size() const;

        /*! Get the value of the prime p.
         *  \return The value of the prime p.
         */
        ptype pvalue() const;

        /*! Get the first item of the internal representation of
         *  the vector.
         *  \return The first item of the internal representation.
         */
        list_item first() const;
        
        /*! Get the last item of the internal representation of 
         *  the vector.
         *  \return The last item of the internal representation.
         */
        list_item last() const;

        /*! Get the successor of an item of the internal representation.
         *  \param it An item.
         *  \return The successor of it.
         */
        list_item succ( list_item it ) const;

        /*! Get the predecessor of an item of the interal representation.
         *  \param it An item.
         *  \return The predecessor of it.
         */
        list_item pred( list_item it ) const;

        /*! Get the index of an item.
         *  \return The index of an item.
         */
        indextype index( list_item it ) const;

        /*! Get the information of an item.
         *  \return The information of an item.
         */
        ptype inf( list_item it ) const;

        private:

        typedef two_tuple<indextype,ptype> entry;
        // list of tuples (tuple = < index, value >)
        list< entry > l;
        ptype p;

        // compare two entries of a spvecfp
        static inline int compare( const entry& a, const entry& b ) {
            return leda::compare( a.first(), b.first() );
        }
    };

    /*! Output a sparse vector to a stream.
     *  \param o The stream to output to.
     *  \param v The sparse vector to output to.
     *  \return The stream after outputing.
     */
    std::ostream& operator<<( std::ostream& o, const spvecfp& v );


} // end of mcb namespace

#endif // SPVECFP_H

