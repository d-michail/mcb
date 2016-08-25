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

/*! \file mcb_approx.C
 *  \brief Approximate MCB construction.
 */

#include <LEP/mcb/spanner.h>
#include <LEP/mcb/ushortpath.h>
#include <LEP/mcb/mcb_approx.h>

namespace mcb { 

#if defined(LEDA_NAMESPACE)
    using leda::graph;
    using leda::edge;
    using leda::edge_map;
    using leda::edge_array;
    using leda::node;
    using leda::node_map;
    using leda::node_array;
#endif

    int UMCB_APPROX( const graph& g,
            const int k,
            array< mcb::spvecgf2 >& mcb,
            const mcb::edge_num& enumb
            )
    {
        unweighted_umcb_approx<mcb::spvecgf2> tmp( g, k, mcb, enumb );
        return tmp.run();
    }

    int UMCB( const graph& g,
            array< mcb::spvecgf2 >& mcb,
            const mcb::edge_num& enumb
            )
    {
        return UMCB_APPROX( g, 1, mcb, enumb );
    }

} // end namespace mcb

/* ex: set ts=4 sw=4 sts=4 et: */
