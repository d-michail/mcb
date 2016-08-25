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


#include <LEP/mcb/fp.h>

namespace mcb { 


// default constructor
spvecfp::spvecfp() {
    this->len = 1; 
    this->p = 1;
}

// constructor
spvecfp::spvecfp( indextype len, const ptype& p ) {  
#if ! defined(LEDA_CHECKING_OFF)
    if ( len <= 0 || p <= 0 ) 
        leda::error_handler(999,"MIN_CYCLE_BASIS: spvecfp: \
length or prime error");
#endif          
    this->p = p;
    this->len = len;
}

// copy constructor
spvecfp::spvecfp( const spvecfp& a ) {
    *this = a;
}

spvecfp::~spvecfp() { 
}

// clear and reset
void spvecfp::reset( indextype len, const ptype& p ) { 
#if ! defined(LEDA_CHECKING_OFF)
    if ( len <= 0 || p <= 0 ) 
        leda::error_handler(999,"MIN_CYCLE_BASIS: spvecfp: \
length or prime error");
#endif  
    l.clear();
    this->len = len;
    this->p = p;
}


// assign a vector to the current vector
spvecfp& spvecfp::operator=( const spvecfp& i ) { 
    l.clear();
    this->p = i.pvalue();
    this->len = i.max_length();

    list_item it = i.first();
    while( it != nil ) { 
        this->l.append( i.l.contents( it ) ); 
        it = i.l.succ( it );
    }
    return *this;
}

// assign current vector to be e_i
spvecfp& spvecfp::operator=( const indextype& i ) {
    l.clear();
    if ( i == 0 ) 
        return *this;
#if ! defined(LEDA_CHECKING_OFF)
    assert( i > 0 && i <= len );
#endif
    l.append( entry(i,1) );
    return *this;
}


// negate
spvecfp spvecfp::operator-() const { 
    spvecfp res ( len, p );
    indextype index;
    ptype v;

    list_item it = l.first();
    while( it != nil ) { 
        index = l.contents( it ).first();
        v = l.contents( it ).second();
        res.l.append( entry( index, -v ) );
        it = l.succ( it );
    }
    return res;
}

// compute inner product
ptype spvecfp::operator*( const spvecfp& a ) const { 
#if ! defined(LEDA_CHECKING_OFF)
    if ( len != a.max_length() ) 
        leda::error_handler(999,"MIN_CYCLE_BASIS: dot \
product of diff length vectors");
    assert( p == a.pvalue() );
#endif
    ptype res = 0;
    indextype index, aindex;
    list_item it = l.first(), ait = a.first();

    // now compute dot product
    while( it != nil && ait != nil ) { 
        index = l.contents( it ).first();
        aindex = a.index( ait );

        if ( index > aindex ) { ait = a.succ( ait ); }
        else if ( index < aindex ) { it = l.succ( it ); }
        else if ( index == aindex ) { 
            ptype v = (l.contents(it).second() * a.inf( ait ) )%p;
            res = ( res + v ) %p;
            it = l.succ( it );
            ait = a.succ( ait );
        }
    }
    return res;
}

spvecfp spvecfp::operator+( const spvecfp& a ) const { 
#if ! defined(LEDA_CHECKING_OFF)
    if ( len != a.max_length() ) 
        leda::error_handler(999,"MIN_CYCLE_BASIS: adding \
different length vectors");
    assert( p == a.pvalue() );
#endif  
    spvecfp res( len, p );
    indextype index, aindex;
    list_item it = l.first(), ait = a.first();

    // now add them
    while( it != nil && ait != nil ) {
        index = l.contents( it ).first();
        aindex = a.index( ait );

        if ( index > aindex ) { 
            res.l.append( a.l.contents( ait ) );
            ait = a.succ( ait );
        }
        else if ( index < aindex ) { 
            res.l.append( l.contents( it ) );
            it = l.succ( it );
        }
        else if ( index == aindex ) { 
            ptype v = (l.contents( it ).second() + a.inf( ait )) % p;
            while( v < 0 )  v += p; // make [-i]_p = [p-i]_p
            while( v >= p ) v -= p; // make [i+p]_p = [i]_p
            if ( v != 0 ) 
                res.l.append( entry( index, v ) );
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

// compute product with a constant
spvecfp spvecfp::operator*( const ptype& a ) {
    spvecfp res( len, p );

    list_item it = l.first();
    while( it != nil  ) { 
        indextype index = l.contents( it ).first();
        ptype v = ( l.contents( it ).second() * a ) % p;

        while( v < 0 )  v += p; // make [-i]_p = [p-i]_p
        while( v >= p ) v -= p; // make [i+p]_p = [i]_p

        if ( v != 0 ) 
            res.l.append( entry( index, v ) );
        it = l.succ( it );
    }
    return res;
}

// += operator
spvecfp& spvecfp::operator+=( const spvecfp& a ) { 
    *this = *this + a;
    return *this;
}

// -= operator
spvecfp& spvecfp::operator-=( const spvecfp& a ) { 
    *this = *this + (-a);
    return *this;
}

// print the vector
void spvecfp::print( std::ostream& o ) const { 
    indextype i = 1, index;
    list_item it = l.first();
    index = (it != nil) ? l.contents( it ).first() : len+1 ;
    while( true ) { 
        while ( index > i ) { 
            o << 0 << " ";
            i++;
        }

        if ( index == i && it != nil ) { 
            o << l.contents( it ).second() << " ";
            it = l.succ( it );
            i++;
        }

        if ( it == nil ) {  // no more elements
            index++;
            while( index++ <= len ) { 
                o << 0 << " ";
            }
            break;
        }
        else 
            index = l.contents( it ).first();
    }
}


// append an entry
// no checking is done to ensure short order
void spvecfp::append( indextype index, const ptype& value ) {
#if ! defined(LEDA_CHECKING_OFF)
    if ( index < 1 || index > len ) 
        leda::error_handler(999,"MIN_CYCLE_BASIS: \
appending on spvecfp: out-of-bounds");
#endif
    l.append( entry( index, value ) );
}


// define access stuff, like leda lists
bool spvecfp::empty() const { return l.empty(); }
void spvecfp::clear() { l.clear(); }
indextype spvecfp::size() const {  return l.size(); }
indextype spvecfp::max_length() const { return len; }
ptype spvecfp::pvalue() const { return p; }
list_item spvecfp::first() const { return l.first(); }
list_item spvecfp::last() const { return l.last(); }
list_item spvecfp::succ( list_item it ) const { return l.succ( it ); }
list_item spvecfp::pred( list_item it ) const { return l.pred( it ); }
indextype spvecfp::index( list_item it ) const { return l.contents( it ).first(); }
ptype spvecfp::inf( list_item it ) const { return l.contents( it ).second(); }

std::ostream& operator<<( std::ostream& o, const spvecfp& v ) { 
    v.print( o );
    return o;
}



} // end of mcb namespace

/* ex: set ts=8 sw=4 sts=4 noet: */


