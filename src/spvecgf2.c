
#line 6220 "MIN_CYCLE_BASIS.lw"
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
// Copyright (C) 2004-2005 - Dimitrios Michail


#line 1120 "MIN_CYCLE_BASIS.lw"
/*! \file spvecgf2.c
 *  \brief Implementation of sparse vector.
 */

#include <LEP/mcb/spvecgf2.h>

namespace mcb { 


#line 1432 "MIN_CYCLE_BASIS.lw"
    //default constructor
    spvecgf2::spvecgf2(  ) { }
    
    // copy constructor
    spvecgf2::spvecgf2( const spvecgf2 & a )
    {
        *this = a;
    }

    spvecgf2::~spvecgf2(  ) {
    }


#line 1452 "MIN_CYCLE_BASIS.lw"
    // assign a vector to the current vector
    spvecgf2& spvecgf2::operator=( const spvecgf2& i )
    {
        l.clear(  );
    
        list_item it = i.first(  );
        while ( it != nil ) {
            this->l.append( i.l.contents( it ) );
            it = i.l.succ( it ); 
        }
        return *this;
    }

#if (__LEDA__ >= 500)
    spvecgf2& spvecgf2::operator=( const d_int_set& i )
#else
    spvecgf2& spvecgf2::operator=( d_int_set& i )
#endif
    {
        l.clear();
        i.get_element_list( l );
        return *this;
    }

    // assign current vector to be e_i
    spvecgf2& spvecgf2::operator=( const int &i ) {
#if ! defined(LEDA_CHECKING_OFF)
        assert( i >= 0 );
#endif
        l.clear(  );
        l.append( i );
        return *this;
    }

    // negate
    spvecgf2 spvecgf2::operator-(  ) const
    {
        return spvecgf2( *this );
    }

    // compute inner product
    int spvecgf2::operator*( const spvecgf2& a ) const
    {
        int res = 0;
        int index, aindex;
        list_item it = l.first(  ), ait = a.first(  );

        // now compute dot product
        while ( it != nil && ait != nil )
        {
            index = l.contents( it );
            aindex = a.l.contents( ait );

            if ( index > aindex )
            {
                ait = a.succ( ait );
            } else if ( index < aindex )
            {
                it = l.succ( it );
            } else if ( index == aindex ) {
                res = ( res + 1 ) % 2;
                it = l.succ( it );
                ait = a.succ( ait );
            }
        }
        return res;
    }

    // compute intersection
    spvecgf2 spvecgf2::intersect( const spvecgf2& a ) const
    {
        spvecgf2 res;
        int index, aindex;
        list_item it = l.first(  ), ait = a.first(  );

        while ( it != nil && ait != nil )
        {
            index = l.contents( it );
            aindex = a.l.contents( ait );

            if ( index > aindex )
            {
                ait = a.succ( ait );
            } else if ( index < aindex )
            {
                it = l.succ( it );
            } else if ( index == aindex ) {
                res.l.append( index );
                it = l.succ( it );
                ait = a.succ( ait );
            }
        }
        return res;
    }

    spvecgf2 spvecgf2::operator+( const spvecgf2& a ) const
    {
        spvecgf2 res;
        int index, aindex;
        list_item it = l.first(  ), ait = a.first(  );

        // now add them
        while ( it != nil && ait != nil )
        {
            index = l.contents( it );
            aindex = a.l.contents( ait );

            if ( index > aindex )
            {
                res.l.append( aindex );
                ait = a.succ( ait );
            } else if ( index < aindex )
            {
                res.l.append( index );
                it = l.succ( it );
            } else if ( index == aindex ) {
                // 1 + 1 = 0, don't add anything
                it = l.succ( it );
                ait = a.succ( ait );
            }
        }

        // append remaining stuff
        while ( it != nil ) {
            res.l.append( l.contents( it ) );
            it = l.succ( it );
        }
        while ( ait != nil ) {
            res.l.append( a.l.contents( ait ) );
            ait = a.succ( ait );
        }

        return res;
    }

    spvecgf2 spvecgf2::operator%( const spvecgf2& a ) const
    {
        return *this + a;
    }

    void spvecgf2::add( const spvecgf2& a )
    {
        int index, aindex;
        list_item it = l.first(  ), ait = a.first(  );

        // now add them
        while ( it != nil && ait != nil )
        {
            index = l.contents( it );
            aindex = a.l.contents( ait );

            if ( index > aindex )
            {
		l.insert( a.l.contents( ait ), it, leda::before );
                ait = a.succ( ait );
            } else if ( index < aindex )
            {
		// don't add until index > aindex
                it = l.succ( it );
            } else if ( index == aindex ) {
		// delete item from list
		list_item itn = l.succ(it);
		l.erase( it );
		it = itn;
                ait = a.succ( ait );
            }
        }

        // append remaining stuff
        while ( ait != nil ) {
	    l.append( a.l.contents( ait ) );
            ait = a.succ( ait );
        }
    }


    // this function is here to help interfacing with the 
    // SIGNED_GRAPH implementation
    d_int_set spvecgf2::operator%( const d_int_set& a ) const
    {
        d_int_set res = a;
        list_item it = l.first();
        int i;
        while( it != nil ) {
            i = l.contents( it );
            if ( res.member( i ) )
                res.del( i );
            else
                res.insert( i );
        }
        return res;
    }


    // += operator
    spvecgf2& spvecgf2::operator+=( const spvecgf2& a ) {
        *this = *this + a;
        return *this;
    }

    // -= operator
    spvecgf2& spvecgf2::operator-=( const spvecgf2& a ) {
        *this = *this + a;
        return *this;
    }

    spvecgf2& spvecgf2::operator%=( const spvecgf2& a ) {
        *this = *this + a;
        return *this;
    }


#line 1672 "MIN_CYCLE_BASIS.lw"
    // transform to d_int_set
    d_int_set spvecgf2::to_d_int_set() const {
        d_int_set S;
        list_item it = l.first();
        while( it != nil ) { 
            S.insert( l.contents(it) );
            it = l.succ(it);
        }
        return S;
    }
    
    // print the vector
    void spvecgf2::print( std::ostream & o ) const
    {
        list_item it = l.first();
        while( it != nil ) { 
            o << l.contents( it );
            if ( l.succ(it) != nil )
                o << " ";
            it = l.succ(it);
        }
    }

    // swap
    void spvecgf2::swap( spvecgf2& a ) { 
	l.swap( a.l );
    }

    // helper function
    void swap( spvecgf2& a, spvecgf2& b ) { 
	a.swap(b);
    }
 
#line 1713 "MIN_CYCLE_BASIS.lw"
    
    // append an entry
    // no checking is done to ensure short order
    void spvecgf2::append( int index ) { 
#if ! defined(LEDA_CHECKING_OFF)
        if ( index < 0 )
            leda::error_handler( 999, "MIN_CYCLE_BASIS: \
appending on spvecgf2: out-of-bounds" );
#endif
        l.append( index );
    }

    void spvecgf2::insert( int index ) { append(index); }

    std::ostream& operator<<( std::ostream& o, const spvecgf2& v )
    {
	if ( v.size() > 0 ) o << "(" << v.size() << ") ";
        v.print( o );
        return o;
    }

    std::istream& operator>>( std::istream& o, spvecgf2& v ) { 
	v.clear();

	char c;
	int size, el, max = 0;
	bool max_defined = false;

	o >> c >> size >> c;
	if ( size > 0 ) {
	    for( int i = 0; i < size; i++ ) { 
		o >> el;
		if ( max_defined == false ) { 
		    max = el; max_defined = true;
		}
		else { 
		    if ( el <= max ) { 
			leda::error_handler(999,"spvecgf2: entries not in increasing order!");
		    }
		    else 
			max = el; 
		}
		v.append( el );
	    }
	}
	return o;
    }


#line 1130 "MIN_CYCLE_BASIS.lw"
} // end namespace mcb

#line 6217 "MIN_CYCLE_BASIS.lw"
/* ex: set ts=8 sw=4 sts=4 noet: */



