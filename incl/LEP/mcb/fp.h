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


/*! \file fp.h
 *  \brief Sparse vector in \f$F_p\f$.
 */

#ifndef FP_H
#define FP_H

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

    
    template<class T>
    class fp { 

        public:
            // extended euclidean gcd algorithm
            static T ext_gcd( T& a, T& b, T& x, T& y );
            static T get_mult_inverse( T& a, T& p );

    };


    // extended euclidean gcd algorithm
    template<class T>
    T fp<T>::ext_gcd( T& a, T& b, T& x, T& y ) { 

        // initialize
        T _x[2], _y[2], _a[2], q;
        bool swap, aneg, bneg;
        indextype i;

        _x[0] = 1; _x[1] = 0; _y[0] = 0; _y[1] = 1;
        aneg = a < 0;
        bneg = b < 0;

        a = ( a < 0 )? -a: a;
        b = ( b < 0 )? -b: b;
        if ( a == 0 ) { y = bneg?-1:1; return b; }
        if ( b == 0 ) { x = bneg?-1:1; return a; }

        // swap arguments appropriately
        _a[0] = a;  
        _a[1] = b;
        swap = false;
        if ( b > a ) { _a[0] = b; _a[1] = a; swap = true; }

        // do the work
        i = 0; 
        while( true ) {
            q = _a[ i ] / _a[ 1 - i ];
            if ( _a[ i ] % _a[ 1 - i ] == 0 ) break;
            _a[ i ] = _a[ i ] % _a[ 1 - i ];
            _x[ i ] = _x[ i ] - q * _x[ 1 - i ];
            _y[ i ] = _y[ i ] - q * _y[ 1 - i ];
            i = 1 - i;
        }

        // did we swap arguments?
        if ( swap ) {
            x = _y[ 1 - i ] * ( aneg?-1:1);
            y = _x[ 1 - i ] * ( bneg?-1:1);
        }
        else {
            x = _x[ 1 - i ] * ( aneg?-1:1 );
            y = _y[ 1 - i ] * ( bneg?-1:1 );
        }

    #if ! defined(LEDA_CHECKING_OFF)
        assert( _a[ 1 - i ] == ( (aneg)?(-a):(a) ) * x + 
                ( (bneg)?(-b):(b) ) * y ); 
    #endif
        return _a[ 1 - i ];
    }


    // compute multiplication inverse of an element
    template<class T>
    T fp<T>::get_mult_inverse( T& a, T& p ) { 
    #if ! defined(LEDA_CHECKING_OFF)
        if ( p <= 0 ) 
            leda::error_handler(999, "MIN_CYCLE_BASIS: p is \
    non-positive");
    #endif
        T x, y;
        if ( fp<T>::ext_gcd( a, p, x, y ) != 1 )
            leda::error_handler(999, "MIN_CYCLE_BASIS: mult inverse\
    does not exist");
        return x;
    }


    template<class T>
    class primes { 

    public:
        // check if a number is prime
        static bool is_prime( const T& p ) { 
            if ( p == T(1) ) return true;
            T t = T(2);
    #if ! defined(LEDA_CHECKING_OFF)
            assert( p >= t );
    #endif
            if ( p % 2 == 0 ) return false;
            T zero = T(0);
            T sqrtt = T( sqrt(p) ) + 1;
    #if ! defined(LEDA_CHECKING_OFF)
            if ( sqrtt * sqrtt < p ) 
                leda::error_handler(999,"MIN_CYCLE_BASIS: is_prime: \
    error calculating square"); 
    #endif
            while( t <= sqrtt ) { 
                if ( p % t == zero ) return false;
                t++;
            }
            return true;
        }
    };




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
     *  \remark Indices are between \f$1\f$ and \f$len\f$ where \f$len\f$ is the length of the vector.
     *
     *  \date 2005
     *  \author Dimitris Michail
     */
    class spvecfp { 
        
        public:

        /*! Default Constructor */
        spvecfp();
        /*! Constructor
         *  \param p Prime number.
         *  \param len The length of the vector.
         */
        spvecfp( indextype len, const ptype& p );
        
        /*! Copy constructor */
        spvecfp( const spvecfp& a );

        /*! Descructor */
        ~spvecfp();

        /*! Clear the vector and reinitialize it.
         *  \param len The length of the vector.
         *  \param p Prime number.
         */
        void reset( indextype len, const ptype& p );

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

        /*! Get the length of the vector.
         *  \return The length of the vector.
         */
        indextype max_length() const;

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
        indextype len;
    };

    /*! Output a sparse vector to a stream.
     *  \param o The stream to output to.
     *  \param v The sparse vector to output to.
     *  \return The stream after outputing.
     */
    std::ostream& operator<<( std::ostream& o, const spvecfp& v );


} // end of mcb namespace

#endif // FP_H

