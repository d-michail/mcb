
//---------------------------------------------------------------------
// File automatically generated using notangle from MIN_CYCLE_BASIS.lw
// 
// mails and bugs: Dimitris Michail <michail@mpi-sb.mpg.de>
//--------------------------------------------------------------------- 
//
// This program can be freely used in an academic environment
// ONLY for research purposes. Any other use is strictly
// prohibited by the author, without an explicit permission.
//
// Note that this program uses the LEDA library, which is
// NOT free. For more details visit Algorithmic Solutions
// at http://www.algorithmic-solutions.com/
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
// Copyright 2004,2005 - Dimitris Michail



#ifndef EDGE_NUM_H
#define EDGE_NUM_H

#include <LEDA/graph.h>
#include <LEDA/edge_array.h>
#include <LEDA/node_set.h>
#include <LEDA/b_queue.h>

namespace mcb { 


/*{\mtext {\bf \Huge Minimum Cycle Basis - Manual}\medskip}*/
/*{\Manpage {edge_num} {} {An edge numbering} {T}}*/

class edge_num
{
/*{\Mdefinition
A numbering |\Mvar| provides a numbering of the edges of an undirected graph 
from $0$ to $m-1$. The numbering is based on an arbitrary spanning tree $T$.
Edges not on $T$ are numbered from $0$ to $m -n + \kappa -1$ where $\kappa$
are the number of connected components of $G$. Edge on $T$ are numbered
from $m-n+\kappa$ to $m-1$.}*/

private:
// variables:
        leda::edge *index;
        leda::edge_array<int> rindex;
        int m,n,k;
// methods:
        void create_numbering( const leda::graph& G );
        int construct_tree( const leda::graph& g, leda::edge_array<bool>& tree );

// disabled:
        edge_num& operator=( const edge_num& );

public:
/*{\Mcreation
}*/
        edge_num ( const leda::graph& G );
/*{\Mcreate creates a |\Mname| for a graph |G|.
}*/
        edge_num ( const edge_num& enumb );
/*{\Mcreate creates a |\Mname| from an edge numbering $enumb$.
}*/
        ~edge_num (void);
/*{\Mdestruct destroy a |\Mname|
}*/

/*{\Moperations
}*/
        int operator()(leda::edge e) const;
/*{\Mfunop Takes edge |e| and returns its number.
}*/
        leda::edge operator()(int i) const;
/*{\Mfunop Takes integer |i| and returns an edge with this number.
}*/
        bool tree( leda::edge e ) const;
/*{\Mfunc Returns whether an edge |e| belongs to the tree used for constructing
the numbering.}*/
        int dim_cycle_space() const;
/*{\Mfunc Returns the dimension of the cycle space. More precisely |m-n+k|, where
|k| is the number of connected components of the graph.}*/
        int num_connected_comp() const;
/*{\Mfunc Returns the number of connected components of the graph.
}*/
/*{\Mimplementation
The |\Mname| is implemented as two arrays, and therefore requires
$O(m)$ space. All operations take constant time except construction
which takes linear time.}*/
};


} // end namespace mcb

#endif

