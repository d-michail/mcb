/* ex: set tabstop=8 shiftwidth=4 softtabstop=4 noexpandtab: */
//---------------------------------------------------------------------
// File automatically generated using notangle from MIN_CYCLE_BASIS.lw
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
// Copyright (C) 2004-2005 Dimitrios Michail



#ifndef MIN_CYCLE_BASIS_H
#define MIN_CYCLE_BASIS_H

#include <LEDA/graph.h>
#include <LEDA/d_int_set.h>
#include <LEDA/edge_array.h>
#include <LEDA/node_array.h>
#include <LEDA/array.h>
#include <LEDA/list.h>
#include <LEDA/sortseq.h>
#include <LEDA/shortest_path.h>
#include <LEDA/tuple.h>
#include <LEP/mcb/SIGNED_GRAPH.h>

// start our namespace
namespace mcb 
{ 

#if defined(LEDA_NAMESPACE)
using leda::graph;
using leda::array;
using leda::edge;
using leda::edge_array;
using leda::d_int_set;
using leda::list;
using leda::sortseq;
using leda::three_tuple;
using leda::two_tuple;
using leda::DIJKSTRA_T;
using leda::node_set;
#endif


/*{\Mtext
  \newpage
  
  {\bf \large Minimum Cycle Basis Algorithms}\medskip

  \settowidth{\typewidth}{|int|}
  \addtolength{\typewidth}{\colsep}
  \computewidths

  Given an undirected graph $G(V,E)$ and a non-negative length function on the edges
  $w: E \mapsto \mathcal{R}_{\ge 0}$, a minimum cycle basis is a set of cycles
  which can generate the cycle space and at the same time has minimum total length.

  Each cycle of the graph is assumed to be a 0-1 vector indexed on the edge set, and
  operations between cycles is performed in GF(2). The length of a cycle basis is 
  the sum of the length of its cycles and the length of a cycle is the sum of the 
  length of its edges.

  The solution of a minimum cycle basis problem can be in the following two forms.
  \begin{enumerate}
        \item A pair |(mcb, proof)| where both are arrays of compressed integer sets, |array< d_int_set>|.
              Each integer sets represents a cycle. In these integer sets, each edge of the graph 
                  is represented by a unique number provided by the edge numbering, |edge_num|.
        
        \item A pair |(mcb, proof)| where both are arrays of lists of edges, |array< list<edge> >|.
              Each element of this array represents a cycle of the minimum cycle basis.
  \end{enumerate}

  Most functions of this section are templates functions. The template parameter |W| can be instantiated
  with any number type. Attention must be paid in order to avoid overflow of values.

  The whole package is protected using a namespace called "mcb", and therefore using 
  anything requires {\em mcb::xxx} or the {\em using namespace mcb} directive.
  
  }*/



// external 
extern bool MCB_verify_basis( const leda::graph& g,
                              leda::array< leda::d_int_set >& mcb,
                              const edge_num& enumb,
                              bool check_cycle_basis 
                            );

extern void MCB_output_basis( const leda::graph& g,
                              leda::array< leda::d_int_set >& C,
                              const edge_num& enumb,
                              leda::array< leda::list<edge> >& mcb );

extern void MCB_output_basis( const leda::graph& g, 
                              const leda::array< leda::list<leda::edge> >& mcb,
                              const edge_num& enumb,
                              leda::array< leda::d_int_set >& C );

extern int MIN_CYCLE_BASIS_DEPINA( const leda::graph& g,
                            leda::array< leda::d_int_set >& mcb,
                            leda::array< leda::d_int_set >& proof,
                            const edge_num& enumb
                            );
/*{\Mfunc $g$ is an undirected unweighted graph. The function computes a Minimum Cycle
Basis $B$ of $g$, that is a cycle basis of $g$ with the smallest length (sum of lengths 
of cycles). The basis is returned as an array of dynamic integer sets, |d_int_set|, 
called $mcb$. \\
Every edge is represented by some integer, by a fixed and arbitrary numbering. This
numbering is represented by $enumb$. \\
The function also returns a certificate of optimality of 
the minimum cycle basis. More precisely a set of linearly independent vectors for 
which cycle $i$ is the shortest cycle in $g$ with odd intersection with the 
proof vector $i$. \\
The function returns the weight of the Minimum Cycle Basis or is undefined
if there were any errors. \\
The running time is $O( m^3 )$ where $m$ are the number of edges of $g$. \\
\precond $g$ is undirected, simple and loopfree\\
}*/

extern int MIN_CYCLE_BASIS_DEPINA( const leda::graph& g,
                            leda::array< leda::d_int_set >& mcb,
                            const edge_num& enumb
                            );

/*{\Mfunc As above, but |proof| is not computed.
}*/

extern int MIN_CYCLE_BASIS_DEPINA( const leda::graph& g,
                            leda::array< leda::list<leda::edge> >& mcb,
                            leda::array< leda::list<leda::edge> >& proof
                            );

/*{\Mfunc As above, but the minimum cycle basis is represented at an array of lists of edges.}*/

extern int MIN_CYCLE_BASIS_DEPINA( const leda::graph& g,
                            leda::array< leda::list<leda::edge> >& mcb
                            );

/*{\Mfunc As above, but |proof| is not computed.
}*/


// unweighted hybrid algorithm
extern void MCB_construct_superset( const leda::graph& G,
                                    leda::sortseq< leda::two_tuple<int,int>, leda::d_int_set >& super, 
                                    const edge_num& enumb);

extern void MCB_construct_unweighted_paths( const leda::graph& G,
        leda::node_array< leda::node_array<leda::edge> >& pred );

extern int MIN_CYCLE_BASIS_HYBRID( const leda::graph& g,
                                   leda::array< leda::d_int_set >& mcb,
                                   leda::array< leda::d_int_set >& proof,
                                   const edge_num& enumb
                                 );

extern int MIN_CYCLE_BASIS_HYBRID( const leda::graph& g,
                                   leda::array< leda::d_int_set >& mcb,
                                   const edge_num& enumb
                                 );

extern int MIN_CYCLE_BASIS_HYBRID( const leda::graph& g,
                                   leda::array< leda::list<leda::edge> >& mcb,
                                   leda::array< leda::list<leda::edge> >& proof
                                 );

extern int MIN_CYCLE_BASIS_HYBRID( const leda::graph& g,
                                   leda::array< leda::list<leda::edge> >& mcb
                                   );



template<class W>
W MIN_CYCLE_BASIS_DEPINA( const graph& g,
                          const edge_array<W>& len,
                          array< d_int_set >& mcb,
                          const edge_num& enumb
                          ) 
/*{\Mfunc As above, but |proof| is not computed.}*/
{ 
        array< d_int_set > proof_temp;
        return MIN_CYCLE_BASIS_DEPINA( g, len, mcb, proof_temp, enumb );
}


template<class W>
W MIN_CYCLE_BASIS_DEPINA( const graph& g,
                          const edge_array<W>& len,
                          array< d_int_set >& mcb,
                          array< d_int_set >& proof,
                          const edge_num& enumb
                          ) 
/*{\Mfunc $g$ is an undirected graph and $len$ is a non-negative length function 
on the edges of $g$. The function computes a Minimum Cycle Basis $B$ of $g$, that 
is a cycle basis of $g$ with the smallest length (sum of lengths of cycles). The 
basis is returned as an array of dynamic integer sets, |d_int_set|, called $mcb$. \\
Every edge is represented by some integer, by a fixed and arbitrary numbering. This
numbering is represented by $enumb$. \\
The function also returns a certificate of optimality of 
the minimum cycle basis. More precisely a set of linearly independent vectors for 
which cycle $i$ is the shortest cycle in $g$ with odd intersection with the 
proof vector $i$. \\
The function returns the weight of the Minimum Cycle Basis or is undefined
if there were any errors. \\
The running time is $O( m^3 )$ where $m$ are the number of edges of $g$. \\
\precond $g$ is undirected, simple and loopfree \\
\precond $len$ is non-negative \\
}*/
{ 
        #if ! defined(LEDA_CHECKING_OFF)
                if ( g.is_undirected() == false )
                        error_handler(999,"MIN_CYCLE_BASIS: illegal graph\
                                          (directed?)");
                if ( Is_Undirected_Simple( g ) == false )
                        error_handler(999,"MIN_CYCLE_BASIS: illegal graph\
                          (parallel,anti-parallel edges or loops?)");
        #endif

        
        #if ! defined(LEDA_CHECKING_OFF)
        edge e1;
        forall_edges( e1 , g ) { 
                if ( len[e1] < 0 ) 
                        error_handler(999,"MIN_CYCLE_BASIS: illegal edge \
                                          (negative weight)");
        }
        #endif

        int N = enumb.dim_cycle_space();
        mcb.resize( N ); // set size of destination array
        proof.resize(N);



        array< d_int_set >& C = mcb;
        array< d_int_set >& S = proof;
        // initialize signed graph
        sgraph<W,leda::bin_heap> sg( g , len, enumb ) ; 

        
        // initialize cycles C and initial sets S's
        for( int i = 0 ; i < N; i++ ) { S[i].insert( i ); }

   
        W min = W(), ret = W();
        // start computing cycles
        for( int k = 0; k < N; k++ ) { 
                ret = sg.get_shortest_odd_cycle( S[k], C[k] ); 
                min+= ret;

                
                for( int l = k+1; l < N; l++ )   {
                      // cycles are smaller than sets, thus it is better
                      // to intersect C and S than S and C
                      if ( (C[k].intersect(S[l])).size() %2 == 1 )   {
                                S[ l ] %= S[k];
                      }
                }


        }

        return min;
}


template<class W>
W MIN_CYCLE_BASIS_DEPINA( const graph& g, 
                          const edge_array<W>& len, 
                          array< list<edge> >& mcb )
/*{\Mfunc As above, but the minimum cycle basis is represented at an array of lists of edges
and |proof| is not computed.
}*/
{
        edge_num enumb(g);
        array< d_int_set > mcb_temp, proof_temp;
        W min = MIN_CYCLE_BASIS_DEPINA( g, len, mcb_temp, proof_temp, enumb );
        // reformat output
        MCB_output_basis( g, mcb_temp, enumb, mcb );
        return min;
}

template<class W>
W MIN_CYCLE_BASIS_DEPINA( const graph& g, 
                          const edge_array<W>& len,
                          array< list<edge> >& mcb,
                          array< list<edge> >& proof) 
/*{\Mfunc As above, but the minimum cycle basis is represented at an array of lists of edges.}*/
{
        edge_num enumb(g);
        array< d_int_set > mcb_temp, proof_temp;
        W min = MIN_CYCLE_BASIS_DEPINA( g, len, mcb_temp, proof_temp, enumb );
        // reformat output
        MCB_output_basis( g, mcb_temp, enumb, mcb );
        MCB_output_basis( g, proof_temp, enumb, proof );
        return min;
}


template<class W>
bool MIN_CYCLE_BASIS_DEPINA_CHECK( const graph& g,
                                   const edge_array<W>& len,
                                   array< d_int_set >& mcb,
                                   array< d_int_set >& proof,
                                   const edge_num& enumb ) 

/*{\Mfunc $g$ is an undirected graph and $len$ is a non-negative length function on 
the edges of $g$. $mcb$ is a minimum cycle basis and $proof$ a certificate of 
optimality of $mcb$. \\
$enumb$ is the numbering on the edges of the graph which was used in order to 
construct the minimum cycle basis. \\
The function checks whether the $mcb$ is indeed a minimum cycle basis. \\
The running time is $O( m^3 )$ where $m$ are the number of edges of $g$. \\
\precond $G$ is undirected, simple and loopfree \\
\precond $len$ is non-negative \\
\precond $enumb$ is the same numbering as the one used to construct the $mcb$.
}*/

{
        #if ! defined(LEDA_CHECKING_OFF)
                if ( g.is_undirected() == false )
                        error_handler(999,"MIN_CYCLE_BASIS: illegal graph\
                                          (directed?)");
                if ( Is_Undirected_Simple( g ) == false )
                        error_handler(999,"MIN_CYCLE_BASIS: illegal graph\
                          (parallel,anti-parallel edges or loops?)");
        #endif

        
        #if ! defined(LEDA_CHECKING_OFF)
        edge e1;
        forall_edges( e1 , g ) { 
                if ( len[e1] < 0 ) 
                        error_handler(999,"MIN_CYCLE_BASIS: illegal edge \
                                          (negative weight)");
        }
        #endif

        sgraph<W,leda::bin_heap> sg( g, len, enumb );

        if ( MCB_verify_basis( g, mcb, enumb, true ) == false ||
             MCB_verify_basis( g, proof, enumb, false ) == false ||
             mcb.size() != proof.size() ) 
                return false;

        // verify cycles
        W cycle_len;
        list<int> L;
        for( int i = 0; i < mcb.size(); i++ ) { 
                d_int_set TEMP;
                W min = sg.get_shortest_odd_cycle( proof[i] , TEMP );

                // compute cycle length
                cycle_len = W();
                L.clear();
                mcb[i].get_element_list(L);
                int el;
                forall( el, L ) cycle_len += len[ enumb(el) ];
        
                if ( cycle_len != min ) 
                        return false;
        }
        return true;
}


template<class W>
bool MIN_CYCLE_BASIS_DEPINA_CHECK( const graph& g,
                                   const edge_array<W>& len,
                                   array< list<edge> >& mcb,
                                   array< list<edge> >& proof 
                                   )
/*{\Mfunc As above, but using lists of edges to represent the cycles.
}*/
{
        edge_num enumb( g );
        array< d_int_set > mcb_temp, proof_temp;
        MCB_output_basis( g, mcb, enumb, mcb_temp );
        MCB_output_basis( g, proof, enumb, proof_temp );
        return MIN_CYCLE_BASIS_DEPINA_CHECK( g, len, mcb_temp, proof_temp, enumb );
}


// construct unique paths from v to u
template<class W>
void MCB_construct_paths( const graph& G,
        const edge_array<W>& len,
        node_array< node_array<edge> >& pred
        )
{
        // initialize
        node v;
        node_array<W> dist(G);
        
        // calculate paths
        forall_nodes( v, G ) {
                pred[v].init( G, nil );
        
                // run dijkstra
                DIJKSTRA_T<W>( G, v, len, dist, pred[v] );
        }
}

      
// construct Horton's superset of MCB
template<class W>
void MCB_construct_superset( const graph& G,
                const edge_array<W>& len,
                sortseq< three_tuple<W,int,int>, d_int_set >& super, 
                const edge_num& enumb)
{
    
    // initialize
    d_int_set C;
    node v,w,u,vz,ezs,ezt; 
    edge e,ez;

    int c = 0;
    node_array<int> degree(G,0);
    node_set touched( G );
    bool dege;

    W min;

    // construct paths
    node_array< node_array<edge> > pred( G );
    MCB_construct_paths<W>( G, len, pred );
        
    // loop - for all nodes
    forall_nodes( v , G ) { 

        // loop - for all edges
        forall_edges( e, G ) { 
            
            dege = false;

            w = G.source(e);
            u = G.target(e);

            // valid?
            if ( v == w || v == u || 
                 pred[ v ][ w ] == nil || pred[ v ][ u ] == nil ||
                 pred[ v ][ w ] == e || pred[ v ][ u ] == e || 
                 pred[ w ][ v ] == e || pred[ u ][ v ] == e ) 
                    continue;

            // try to construct cycle
            C.clear();

            // insert e
            C.insert( enumb(e) );
            ezs = G.source( e );
            ezt = G.target( e );
            degree[ ezs ]++;
            degree[ ezt ]++;
            touched.insert( ezs );
            touched.insert( ezt );



            min = len[ e ];

            // create two paths
            int en;
            node x,y; 

            // make sure path is unique
            if ( v->id() < w->id() ) { y = v; x = w; }
            else { y = w; x = v; }
            
            while( pred[ y ][ x ] != nil && dege == false ) {
                    ez = pred[ y ][ x ];
                    en = enumb( ez );
                    if ( ! C.member( en ) ) {
                            C.insert( en );
                            ezs = G.source( ez );
                            ezt = G.target( ez );
                            degree[ ezs ]++;
                            degree[ ezt ]++;
                            if ( degree[ ezs ] > 2 || degree[ ezt ] > 2 )
                                    dege = true;
                            touched.insert( ezs );
                            touched.insert( ezt );
                            min += len[ ez ];
                    }
                    x = G.opposite( ez, x );
            }
 
            if ( v->id() < u->id() ) { y = v; x = u; }
            else { y = u; x = v; }
            
            while( pred[ y ][ x ] != nil && dege == false ) {
                    ez = pred[ y ][ x ];
                    en = enumb( ez );
                    if ( ! C.member( en ) ) {
                            C.insert( en );
                            ezs = G.source( ez );
                            ezt = G.target( ez );
                            degree[ ezs ]++;
                            degree[ ezt ]++;
                            if ( degree[ ezs ] > 2 || degree[ ezt ] > 2 )
                                    dege = true;
                            touched.insert( ezs );
                            touched.insert( ezt );
                            min += len[ ez ];
                    }
                    x = G.opposite( ez, x );
            }
 
                        
            // init degree array, and check for degeneracy
            while( touched.empty() == false ) {
                vz = touched.choose();
                touched.del( vz );
                if ( degree[ vz ] == 1 ) dege = true;
                degree[ vz ] = 0;
            }

            if ( dege == true ) continue;

            // add cycle to superset
            three_tuple<W,int,int> tt( min, C.size(), c++ ); 
            super.insert( tt, C );
        }
    }
}


template<class W>
W MIN_CYCLE_BASIS_HYBRID( const graph& g,
                          const edge_array<W>& len,
                          array< d_int_set >& mcb,
                          array< d_int_set >& proof,
                          const edge_num& enumb
                          ) 
/*{\Mfunc $g$ is an undirected and $len$ is a non-negative length function 
on the edges of $g$. The function computes a Minimum Cycle Basis $B$ of $g$, that 
is a cycle basis of $g$ with the smallest length (sum of lengths of cycles). The 
basis is returned as an array of dynamic integer sets, |d_int_set|, called $mcb$. \\
Every edge is represented by some integer, by a fixed and arbitrary numbering. This
numbering is represented by $enumb$. \\
The function also returns a certificate of optimality of 
the minimum cycle basis. More precisely a set of linearly independent vectors for 
which cycle $i$ is the shortest cycle in $g$ with odd intersection with the 
proof vector $i$. \\
The function returns the weight of the Minimum Cycle Basis or is undefined
if there were any errors. \\
The running time is $O( m^2 n^2 )$ where $m$ are the number of edges of $g$ 
and $n$ the number of vertices. \\
\precond $g$ is undirected, simple and loopfree \\
\precond $len$ is non-negative \\
}*/
{ 

        #if ! defined(LEDA_CHECKING_OFF)
                if ( g.is_undirected() == false )
                        error_handler(999,"MIN_CYCLE_BASIS: illegal graph\
                                          (directed?)");
                if ( Is_Undirected_Simple( g ) == false )
                        error_handler(999,"MIN_CYCLE_BASIS: illegal graph\
                          (parallel,anti-parallel edges or loops?)");
        #endif

        
        #if ! defined(LEDA_CHECKING_OFF)
        edge e1;
        forall_edges( e1 , g ) { 
                if ( len[e1] < 0 ) 
                        error_handler(999,"MIN_CYCLE_BASIS: illegal edge \
                                          (negative weight)");
        }
        #endif

        int N = enumb.dim_cycle_space();
        mcb.resize( N ); // set size of destination array
        proof.resize(N);



        array< d_int_set >& C = mcb;
        array< d_int_set >& S = proof;
        
        // initialize cycles C and initial sets S's
        for( int i = 0 ; i < N; i++ ) { S[i].insert( i ); }

   
        // construct superset
        sortseq< three_tuple<W,int,int> ,d_int_set> super( compare );
        MCB_construct_superset<W>( g, len, super, enumb );
        
        W min = W(), ret = W();
        leda::seq_item s;
        // start computing cycles
        for( int k = 0; k < N; k++ ) { 
                
                // compute shortest odd cycle
                s = super.min_item();
                bool f = false;
                while ( s != nil ) { 
                        if ( ( (super[s]).intersect( S[k] ) ).size() % 2 == 1 ) { // is odd
                                C[k] = super[s];
                                ret = super.key( s ).first();
                                super.del_item( s );
                                f = true; // found
                                break;
                        }
                        s = super.succ( s );
                }
                if ( f == false )
                        error_handler(999,"MIN_CYCLE_BASIS (HYBRID):\
                        superset contains no MCB :(");
                min += ret;

                
                for( int l = k+1; l < N; l++ )   {
                      // cycles are smaller than sets, thus it is better
                      // to intersect C and S than S and C
                      if ( (C[k].intersect(S[l])).size() %2 == 1 )   {
                                S[ l ] %= S[k];
                      }
                }

        }

        return min;
}


template<class W>
W MIN_CYCLE_BASIS_HYBRID( const graph& g,
                          const edge_array<W>& len,
                          array< d_int_set >& mcb,
                          const edge_num& enumb
                          )
/*{\Mfunc As above, but |proof| is not computed.}*/
{
        array< d_int_set > proof_temp;
        return MIN_CYCLE_BASIS_HYBRID( g, len, mcb, proof_temp, enumb );
}

template<class W>
W MIN_CYCLE_BASIS_HYBRID( const graph& g,
                          const edge_array<W>& len,
                          array< list<edge> >& mcb,
                          array< list<edge> >& proof
                          )
/*{\Mfunc As above, but the minimum cycle basis is represented as an array of lists of edges.}*/
{
        array< d_int_set > mcb_temp, proof_temp;
        edge_num enumb( g );
        W min = MIN_CYCLE_BASIS_HYBRID( g, len, mcb_temp, proof_temp, enumb );
        MCB_output_basis( g, mcb_temp, enumb, mcb );
        MCB_output_basis( g, mcb_temp, enumb, proof );
        return min;
}

template<class W>
W MIN_CYCLE_BASIS_HYBRID( const graph& g,
                          const edge_array<W>& len,
                          array< list<edge> >& mcb
                          )
/*{\Mfunc As above, but the minimum cycle basis is represented as an array of lists of 
edges and |proof| is not computed.}*/
{
        array< d_int_set > mcb_temp, proof_temp;
        edge_num enumb( g );
        W min = MIN_CYCLE_BASIS_HYBRID( g, len, mcb_temp, proof_temp, enumb );
        MCB_output_basis( g, mcb_temp, enumb, mcb );
        return min;
}


} // namespace mcb end

#endif  // MIN_CYCLE_BASIS_H

