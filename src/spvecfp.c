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


#include <LEP/mcb/spvecfp.h>

namespace mcb { 


// default constructor
spvecfp::spvecfp() {
    this->p = 3;
}

// constructor
spvecfp::spvecfp( const ptype& p ) {  
#if ! defined(LEDA_CHECKING_OFF)
    if ( p <= 0 ) 
        leda::error_handler(999,"MIN_CYCLE_BASIS: spvecfp: prime error");
#endif          
    this->p = p;
}

// copy constructor
spvecfp::spvecfp( const spvecfp& a ) {
    *this = a;
}

spvecfp::~spvecfp() { 
}

// clear and reset
void spvecfp::reset( const ptype& p ) { 
#if ! defined(LEDA_CHECKING_OFF)
    if ( p <= 0 ) 
        leda::error_handler(999,"MIN_CYCLE_BASIS: spvecfp: prime error");
#endif  
    l.clear();
    this->p = p;
}


// assign a vector to the current vector
spvecfp& spvecfp::operator=( const spvecfp& i ) { 
    l.clear();
    this->p = i.pvalue();

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
#if ! defined(LEDA_CHECKING_OFF)
    assert( i >= 0 );
#endif
    l.append( entry(i,1) );
    return *this;
}


// negate
spvecfp spvecfp::operator-() const { 
    spvecfp res ( p );
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
    assert( p == a.pvalue() );
#endif  
    spvecfp res( p );
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
    spvecfp res( p );

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
    indextype i = 0, index;
    list_item it = l.first();
    if ( it == nil ) return;
    index = l.contents( it ).first();
    while( true ) { 
        while ( index > i ) { 
            o << 0 << " ";
            i++;
        }

        if ( index == i ) { 
            o << l.contents( it ).second() << " ";
            it = l.succ( it );
            i++;
        }

        if ( it == nil ) {  // no more elements
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
    if ( index < 0 )
        leda::error_handler(999,"MIN_CYCLE_BASIS: appending on spvecfp: out-of-bounds");
#endif
    l.append( entry( index, value ) );
}

// sort entries by index
void spvecfp::sort() {
    l.sort( spvecfp::compare );
}


// define access stuff, like leda lists
bool spvecfp::empty() const { return l.empty(); }
void spvecfp::clear() { l.clear(); }
indextype spvecfp::size() const {  return l.size(); }
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


