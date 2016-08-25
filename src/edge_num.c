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


#include <LEP/mcb/edge_num.h>

namespace mcb { 

#if defined(LEDA_NAMESPACE)
using leda::graph;
using leda::edge;
using leda::edge_array;
using leda::node;
using leda::node_set;
using leda::b_queue;
using leda::error_handler;
#endif



edge_num::edge_num( const graph& G ): 
                        m(G.number_of_edges()), 
                        n(G.number_of_nodes()), 
                        rindex(G) { 
#if ! defined(LEDA_CHECKING_OFF)
        if ( G.is_undirected() == false )
                error_handler(999,"edge_num: illegal graph (directed?)");
#endif
        index = new edge [ m ];
        create_numbering( G );
}

edge_num::edge_num( const edge_num& enumb ) { 
        m = enumb.m;
        n = enumb.n;
        k = enumb.k;
        index = new edge [ m ];
        for( int i = 0 ; i < m ; i++ ) 
                index[ i ] = enumb.index[ i ];
        rindex = enumb.rindex;
}

edge_num::~edge_num( void ) {
        if ( index != nil ) 
                delete [] index;
}

int edge_num::operator()(edge e) const { 
        return rindex[e];
}

edge edge_num::operator()(int i) const { 
#if ! defined(LEDA_CHECKING_OFF)
        if ( i < 0 || i > m )
                error_handler(999,"edge_num: illegal number requested");
#endif
        return index[ i ];
}

bool edge_num::tree( edge e ) const { 
        return ( rindex[e] >= m - n + k );
}

int edge_num::dim_cycle_space() const {
        return m - n + k;
}

int edge_num::num_connected_comp() const { 
        return k;
}

void edge_num::create_numbering( const graph& G ) { 
        edge_array<bool> tree( G );
        k = construct_tree( G, tree );
        int N = m - n + k;

        edge e;
        int y = 0 , l = N ;
        forall_edges( e , G ) { 
                if ( tree[ e ] == false ) { 
                        index[y] = e; 
                        rindex[ e ] = y; 
                        y++; 
                }
                else { 
                        index[l] = e; 
                        rindex[ e ] = l; 
                        l++; 
                }
        }
}

int edge_num::construct_tree( const graph& g, edge_array<bool>& tree ) { 
        // initialize
        node v,u,w;
        if ( g.number_of_nodes() == 0 ) 
                return 0;
        b_queue<node> Q( g.number_of_nodes() );
        node_set unreached( g );
        tree.init( g, false );
        forall_nodes(v,g) unreached.insert( v );
        edge e;
        int c = 0;

        // run
        while( unreached.empty() == false ) { 

                v = unreached.choose();
                unreached.del( v );
                Q.append( v );

                while( Q.empty() == false ) { 
                        u = Q.pop();

                        forall_inout_edges( e, u ) { 
                                w = g.opposite( u,e );
                                if( unreached.member(w) == false ) 
                                        continue;

                                unreached.del( w ); 
                                tree[e] = true;
                                Q.append( w );
                        }
                }
                c++;
                
        }
        return c;
}


} // end namespace mcb

