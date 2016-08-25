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

#include <LEP/mcb/config.h>

#ifdef LEDA_GE_V5
#include <LEDA/graph/graph.h>
#include <LEDA/graph/edge_array.h>
#include <LEDA/graph/graph_misc.h>
#include <LEDA/core/d_int_set.h>
#include <LEDA/system/error.h>
#else
#include <LEDA/graph.h>
#include <LEDA/edge_array.h>
#include <LEDA/graph_misc.h>
#include <LEDA/d_int_set.h>
#include <LEDA/error.h>
#endif

#include <LEP/mcb/edge_num.h>
#include <LEP/mcb/spvecgf2.h>
#include <LEP/mcb/umcb.h>

namespace mcb 
{ 

#if defined(LEDA_NAMESPACE)
    using leda::graph;
    using leda::edge;
    using leda::edge_array;
    using leda::array;
    using leda::d_int_set;
    using leda::error_handler;
#endif

    int UMCB_HYBRID( const graph& g, 
            array< d_int_set>& mcb, 
            array< d_int_set>& proof,
            const mcb::edge_num& enumb )
    {
        leda::edge_array<int> len(g,1);
        return UMCB_HYBRID( g, len, mcb, proof, enumb );
    }

    int UMCB_HYBRID( const graph& g,
            array< d_int_set >& mcb,
            const mcb::edge_num& enumb
            )
    {
        array< d_int_set > proof_temp;
        return UMCB_HYBRID( g, mcb, proof_temp, enumb );
    }

} // namespace mcb end

/* ex: set ts=4 sw=4 sts=4 et: */


