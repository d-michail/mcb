
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

/*! \file fp.h
 *  \brief Helper functions
 */

#ifndef FP_H
#define FP_H

#include <iostream>
#include <LEP/mcb/config.h>

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


} // end of mcb namespace

#endif // FP_H

