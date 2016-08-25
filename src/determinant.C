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
// $Id: transform.C 6781 2008-02-20 00:10:13Z dmichail $ 
//

#include <LEP/mcb/config.h>

#ifdef LEDA_GE_V5
#include <LEDA/system/error.h>
#include <LEDA/numbers/integer_matrix.h>
#else
#include <LEDA/error.h>
#include <LEDA/integer_matrix.h>
#endif

namespace mcb 
{ 

#if defined(LEDA_NAMESPACE)
using leda::error_handler;
using leda::integer_matrix;
#endif

    /*  Output a LEDA integer_matrix in a format compatible with maple. 
     *  \param out The output stream
     *  \param B The matrix to output
     *  \ret The output stream after outputing.
     */ 
    std::ostream& output_maple_format( std::ostream& out, const integer_matrix& B )
    {
        out << "[";
        for( int i = 0; i < B.dim1(); ++i )  {
            out << "[";
            for( int j = 0; j < B.dim2(); ++j ) {
                out << B[i][j];
                if ( j < B.dim2()-1 ) 
                    out << ",";
            }
            out << "]";       
            if ( i < B.dim1()-1 ) 
                out << "," << std::endl;
        }
        out << "]" << std::endl;
        return out;
    }




} // namespace mcb end

/* ex: set ts=4 sw=4 sts=4 et: */
