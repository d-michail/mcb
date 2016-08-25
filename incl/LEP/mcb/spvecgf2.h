
//---------------------------------------------------------------------
// File automatically generated using notangle from DMIN_CYCLE_BASIS.lw
//
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


/*! \file spvecgf2.h
 *  \brief Definitions of sparse vector.
 */

#ifndef SPVECGF2_H
#define SPVECGF2_H

#include <iostream>
#include <LEP/mcb/config.h>

#ifdef LEDA_GE_V5
#include <LEDA/system/basic.h>
#include <LEDA/core/list.h>
#include <LEDA/core/d_int_set.h>
#include <LEDA/system/error.h>
#include <LEDA/system/assert.h>
#else
#include <LEDA/basic.h>
#include <LEDA/list.h>
#include <LEDA/d_int_set.h>
#include <LEDA/error.h>
#include <LEDA/assert.h>
#endif
#include <LEP/mcb/edge_num.h>
#include <LEP/mcb/arithm.h>

namespace mcb
{

#if defined(LEDA_NAMESPACE)
    using leda::list;
    using leda::list_item;
    using leda::error_handler;
    using leda::d_int_set;
#endif


    /*! \brief A sparse vector with elements in GF2.
     *
     *  An object of this class represents a cycle of an undirected graph. It behaves as a 
     *  sparse matrix of values in GF(2).
     *
     *  This class is implemented internally by double linked lists. The internal representation
     *  is a list of integers representing the indexes where the vector has a one. The indexes are
     *  stored in increasing order.
     *  Operations append and insert take constant time but do not ensure the correct ordering of 
     *  the elements. See sort() for reconstructing this ordering.
     *
     *  Most binary operations take time proportional to the number of elements in the list by 
     *  assumming that the indexes are stored in increasing order.
     *  Size, empty, clear and the iterators' methods take constant time.
     *  Swap also takes constant time.
     *
     *  Such an object can be accessed in exactly the same manner that a list of LEDA is accessed. 
     *  Each one in the sparse vector corresponds in a list_item in a list. Thus, the following is 
     *  valid:
     *  \code
     *  mcb::spvecgf2 v;
     *  int i;
     *  leda::list_item li = v.first();
     *  while( li != NULL ) { 
     *      i = v.index( li );
     *      // do something with i
     *      li = v.succ( li );
     *  }
     *  \endcode
     *
     *  LEDA's forall macro works for this objects. It can be used in the following way:
     *  \code
     *  mcb::spvecgf2 v;
     *  int i;
     *  forall(i,v) { 
     *      // do something with i
     *  }
     *  \endcode
     *
     *  \date 2005
     *  \author Dimitrios Michail
     */
    class spvecgf2
    {
      public:
        //@{
        /*! Default constructor. */
        spvecgf2();
        
        /*! Copy constructor. */
        spvecgf2( const spvecgf2 & a );

        /*! Destructor */
         ~spvecgf2(  );
        //@}


        //@{
        /*! Assign a vector to another vector. 
         *  \param i The vector to assign from.
         *  \return The current vector after the assignment.
         */
        spvecgf2& operator=( const spvecgf2& i );

        /*! Assign current vector to be \f$e_i\f$. */
        spvecgf2& operator=( const int& i );

        /*! Assignment operator for a leda::d_int_set. 
         *  \param i A compressed integer set of LEDA.
         *  \return The currect sparse vector.
         */
#if (__LEDA__ >= 500)
        spvecgf2& operator=( const d_int_set& i );
#else
        spvecgf2& operator=( d_int_set& i );
#endif

        /*! Negate the current vector.
         *  \return A new object representing the current vector negated. 
         */
        spvecgf2 operator-(  ) const;

        /*! Compute inner product. 
         *  \param a Input vector a.
         *  \return The inner product of this object and a.
         *  \pre Assumes a sorted representation. See insert(), append() and sort().
         */
        int operator*( const spvecgf2& a ) const;

        /*! Operator to add the current vector with another vector.
         *  \param a The second vector to add.
         *  \return A new vector representing the sum.
         *  \pre Assumes a sorted representation. See insert(), append() and sort().
         */
        spvecgf2 operator+( const spvecgf2& a ) const;

        /*! Add a vector to the current vector.
         *  \param a The vector to add.
         *  \pre Assumes a sorted representation. See insert(), append() and sort().
         */
        void add( const spvecgf2& a ); 

        /*! Operator to compute the symmetric difference of this sparse
         *  vector and of a.
         *  \param a A sparse vector.
         *  \return The symmetric difference as a new sparse vector.
         *  \pre Assumes a sorted representation. See insert(), append() and sort().
         */
        spvecgf2 operator%( const spvecgf2& a ) const;
        
        /*! Operator to compute the symmetric difference of this sparse
         *  vector and of a LEDA dynamic integer set.
         *  \param a A dynamic integer set.
         *  \return The symmetric difference as a new dynamic integer set.
         */
        d_int_set operator%( const d_int_set& a ) const;
        
        /*! Compute the intersection of this vector and a.
         *  \param a A sparse vector.
         *  \return The intersection of the current object and a.
         *  \pre Assumes a sorted representation. See insert(), append() and sort().
         */
        spvecgf2 intersect( const spvecgf2& a ) const;

        /*! Add a vector to the current vector.
         *  \param a The vector to add.
         *  \return The current vector after the addition.
         *  \pre Assumes a sorted representation. See insert(), append() and sort().
         */
        spvecgf2& operator+=( const spvecgf2& a );

        /*! Add a vector to the current vector.
         *  \param a The vector to add.
         *  \return The current vector after the addition.
         *  \remark Subtracting and addition is the same in GF(2).
         *  \pre Assumes a sorted representation. See insert(), append() and sort().
         */
        spvecgf2& operator-=( const spvecgf2& a );

        /*! Assign to the current vector the symmetric difference of the 
         *  current vector an vector a.
         *  \param a An input sparse vector.
         *  \return The current vector after the symmetric difference operation.
         *  \pre Assumes a sorted representation. See insert(), append() and sort().
         */
        spvecgf2& operator%=( const spvecgf2& a );

        /*! Swap two sparse vectors.
         *  \param a A sparse vector to swap with the current object.
         *  \remark This is a constant time operation.
         */
        void spvecgf2::swap( spvecgf2& a );
        //@}

        //@{
        /*! Transform the sparse vector to a LEDA dynamic integer set. 
         *  \return A dynamic integer set.
         */
        d_int_set to_d_int_set() const;

        /*! Transform the sparse vector to a list of integers.
         *  \return A LEDA list of integers.
         */
        list<int> to_list() const { return l; }
        //@}

        //@{
        /*! Clear the set. */
        void clear( ) { l.clear(); }

        /*! Print a sparse vector to a stream. 
         *  \param o The stream to print to.
         */
        void print( std::ostream & o ) const;

        /*! Append an entry, without checking for proper order.
         *  \param index An integer representing that the sparse vector has 
         *               a one at that position. Indexing should start from zero.
         *  \remark The internal representation of this object is an ordered list of integers.
         *          This function however makes no attempt to ensure such an ordering, it 
         *          only appends the new entry into the list. Use wisely.
         *  \sa sort()
         */
        void append( int index );

        /*! Append an entry, without checking for proper order.
         *  \param index An integer representing that the sparse vector has 
         *               a one at that position. Indexing should start from zero.
         *  \remark The internal representation of this object is an ordered list of integers.
         *          This function however makes no attempt to ensure such an ordering, it 
         *          only appends the new entry into the list. Use wisely.
         *  \sa sort()
         */
        void insert( int index );

        /*! Delete the first entry of the sparse vector and return its index. 
         *  \return The index of the first entry which was deleted.
         *  \pre The sparse array should not be empty.
         */
        int pop() { return l.pop(); }

        /*! Sort the internal representation of the sparse vector. This operation
         *  ensures that a correct representation after its call. Should not be 
         *  necessary unless insert or append were called in wrong order.
         */
        void sort() { l.sort(); }


        // define access stuff, like leda lists

        /*! Check if empty. 
         *  \return True if empty, false otherwise.
         */
        bool empty() const { return l.empty(); }
        
        /*! Get size of sparse vector.
         *  \return The size of the sparse vector. 
         */
        int size(  ) const { return l.size(); }
        //@}

        //@{
        /*! Returns the first item of the sparse vector. Each item represents 
         *  a position where the sparse vector has a one.
         *  \return Returns the first item of the vector. This item is a 
         *  list_item of LEDA's list representation. NULL is returned if the 
         *  vector contains no elements.
         */
        list_item first(  ) const { return l.first(); }

        /*! Returns the last item of the sparse vector.  Each item represents 
         *  a position where the sparse vector has a one.
         *  \return Returns the last item of the vector. This item is a 
         *  list_item of LEDA's list representation. NULL is returned if the 
         *  vector contains no elements.
         */
        list_item last(  ) const { return l.last(); }

        /*! Get the successor of an item. Each item represents 
         *  a position where the sparse vector has a one.
         *  \return The successor of an item, NULL if sparse vector is empty.
         *  \pre it is an item of the current object.
         */
        list_item succ( list_item it ) const { return l.succ(it); }
        
        /*! Get the predecessor of an item. Each item represents 
         *  a position where the sparse vector has a one.
         *  \return The predecessor of an item, NULL if sparse vector is empty.
         *  \pre it is an item of the current object.
         */
        list_item pred( list_item it ) const { return l.pred(it); }
        
        /*! Get the index of an item. Each item represents a one in the 
         *  sparse vector.
         *  \param it The item.
         *  \return The position of this one in the sparse vector.
         */
        int index( list_item it ) const { return l.contents(it); }

        /*! Get the index of an item. Each item represents a one in the 
         *  sparse vector.
         *  \param it The item.
         *  \return The position of this one in the sparse vector.
         */
        int inf( list_item it ) const { return l.contents(it); }

        // allow the LEDA forall() macro to work
        typedef list_item item;

        item first_item()       const { return l.first(); }
        item last_item()        const { return l.last(); }
        item next_item(item it) const { return it ? l.succ(it) : 0; }
        //@}

      private:

        list < int > l;
    };

    /*! Output a sparse vector.
     *  \param o The stream to output to.
     *  \param v The sparse vector to output.
     *  \return The stream after output.
     */
    std::ostream & operator<<( std::ostream & o, const spvecgf2& v );
    
    /*! Input a sparse vector.
     *  \param o The stream to input from.
     *  \param v Where to store the sparse vector.
     *  \return The stream after reading.
     */
    std::istream & operator>>( std::istream & o, spvecgf2& v );
    
    // swap
    void swap( spvecgf2&, spvecgf2& );



}

#endif

/* ex: set ts=8 sw=4 sts=4 noet: */


