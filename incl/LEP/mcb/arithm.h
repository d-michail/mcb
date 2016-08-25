
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

/*! \file arithm.h
 *  \brief Basic arithmetic definitions.
 */

#ifndef ARITHM_H
#define ARITHM_H

#include <LEP/mcb/config.h>

#ifdef LEDA_GE_V5
#include <LEDA/numbers/integer.h>
#else
#include <LEDA/integer.h>
#endif

namespace mcb { 

// define the types used in our implementation

/*! The index type used for the directed cycle basis algorithm.
 */
typedef int indextype;
typedef short etype;
/*! The prime type used for the directed cycle basis algorithm.
 */
typedef leda::integer ptype;

// give compare for ptype inside mcb namespace
inline int compare(const ptype& x, const ptype& y)  
{ 
    if (x < y) return -1;                              
    else if (x > y) return 1;                          
    else return 0;                                     
} 

} // end of namespace mcb

#endif // ARITHM_H

/* ex: set ts=8 sw=4 sts=4 noet: */



