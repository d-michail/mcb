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



#include <LEDA/graph.h>
#include <LEDA/array.h>
#include <LEDA/list.h>
#include <LEDA/node_set.h>
#include <LEDA/b_queue.h>
#include <LEDA/edge_array.h>
#include <LEDA/graph_misc.h>
#include <LEDA/d_int_set.h>
#include <LEDA/error.h>
#include <LEDA/basic_graph_alg.h>
#include <LEDA/sortseq.h>
#include <LEDA/tuple.h>
#include <LEP/mcb/edge_num.h>
#include <LEP/mcb/SIGNED_GRAPH.h>

namespace mcb 
{ 

#if defined(LEDA_NAMESPACE)
using leda::graph;
using leda::node;
using leda::node_array;
using leda::node_set;
using leda::edge;
using leda::edge_array;
using leda::array;
using leda::d_int_set;
using leda::list;
using leda::b_queue;
using leda::error_handler;
using leda::sortseq;
using leda::two_tuple;
using leda::compare;
#endif


void MCB_output_basis( const graph& g,
                       array< d_int_set >& C,
                       const edge_num& enumb,
                       array< list<edge> >& mcb )
{
        // create cycles of MCB
        list<int> L;
        int en;
        mcb.resize( C.size() );
        for( int i = 0; i < C.size(); ++i ) { 
                L.clear();
                C[i].get_element_list( L );
                forall( en, L )
                        mcb[i].append( enumb(en) );
        }
}

// and reverse, from lists to dynamic sets
void MCB_output_basis( const graph& g,
                       const array< list<edge> >& mcb,
                       const edge_num& enumb,
                       array< d_int_set >& C )
{
        edge e;
        C.resize ( mcb.size() );
        for( int i = 0; i < mcb.size(); ++i )  { 
                forall( e, mcb[i] ) { 
                        C[i].insert( enumb(e) );
                }
        }
}





bool MCB_verify_basis( const graph& g, 
                       array< d_int_set >& mcb,
                       const edge_num& enumb,
                       bool check_cycle_basis )
{
        edge e;
        node v;

        if ( check_cycle_basis ) { 

                // check number of vectors
                int N = enumb.dim_cycle_space();
                if ( mcb.size() != N ) return false;
        
                // check that vectors are cycles
                // TODO: can be done faster
                list<int> L;
                for( int i = 0; i < N; i++ ) { 
                        if ( mcb[i].size() == 0 ) return false;
                        
                        node_array<int> degree(g, 0);
                        L.clear();
                        mcb[i].get_element_list( L );
                        int el;
                        
                        forall( el, L ) { 
                                e = enumb(el);
                                degree[ g.source(e) ]++;
                                degree[ g.target(e) ]++;
                        }
                        
                        forall_nodes( v, g ) 
                                if ( degree[v] % 2 == 1 ) return false;
                }
        }
        
        if ( mcb.size() == 0 ) return true;
        
        // now check for linear independence

        // copy cycles
        array< d_int_set > a(mcb.size());
        for( int i = 0; i < mcb.size(); ++i ) a[i] = mcb[i];

        // do gauss elimination
        for( int i = 0; i < a.size(); ++i) {
                // find non-zero
                if ( a[i].member( i ) == false ) {
                        for( int j = i+1; j < a.size(); ++j ) { 
                                if ( a[j].member(i) == true ) {
                                        d_int_set Stemp = a[i];
                                        a[i] = a[j]; 
                                        a[j] = Stemp;
                                }
                        }
                }

                // no non-zero found
                if ( a[i].member( i ) == false ) return false;

                // 
                for( int j = i + 1; j < a.size(); ++j ) { 
                        if ( a[j].member( i ) == true ) 
                                a[j] %= a[i];
                }
        }

        return true;
}

// verify basis in lists form
bool MCB_verify_basis( const graph& g, 
                       array< list<edge> >& mcb,
                       bool check_cycle_basis )
{
        array< d_int_set > C;
        edge_num enumb( g );
        MCB_output_basis( g, mcb, enumb, C );
        return MCB_verify_basis( g, C, enumb, check_cycle_basis );
}




// compute MCB with d_int_set output for 
// unweighted undirected graphs
int MIN_CYCLE_BASIS_DEPINA( const graph& g,
                            array< d_int_set >& mcb,
                            array< d_int_set >& proof,
                            const edge_num& enumb
                          ) { 

        #if ! defined(LEDA_CHECKING_OFF)
                if ( g.is_undirected() == false )
                        error_handler(999,"MIN_CYCLE_BASIS: illegal graph\
                                          (directed?)");
                if ( Is_Undirected_Simple( g ) == false )
                        error_handler(999,"MIN_CYCLE_BASIS: illegal graph\
                          (parallel,anti-parallel edges or loops?)");
        #endif

        int N = enumb.dim_cycle_space();
        mcb.resize( N ); // set size of destination array
        proof.resize(N);



        array< d_int_set >& C = mcb;
        array< d_int_set >& S = proof;
        // initialize signed graph
        sgraph<int,leda::bin_heap> sg( g, enumb ) ; 

        
        // initialize cycles C and initial sets S's
        for( int i = 0 ; i < N; i++ ) { S[i].insert( i ); }

   
        int min = 0, ret = 0;
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

// compute MCB and proof with d_int_set
// for unweighted undirected graphs
int MIN_CYCLE_BASIS_DEPINA( const graph& g,
                            array< d_int_set >& mcb,
                            const edge_num& enumb
                          ) { 
        array< d_int_set > proof;
        return MIN_CYCLE_BASIS_DEPINA( g, mcb, proof, enumb );
}



// lists version with proof
int MIN_CYCLE_BASIS_DEPINA( const graph& g,
                            array< list<edge> >& mcb,
                            array< list<edge> >& proof) { 
        array< d_int_set > mcb_temp, proof_temp;
        edge_num enumb( g );
        int w = MIN_CYCLE_BASIS_DEPINA( g, mcb_temp, proof_temp, enumb );
        MCB_output_basis( g, mcb_temp, enumb, mcb );
        MCB_output_basis( g, proof_temp, enumb, proof );
        return w;
}

// lists version
int MIN_CYCLE_BASIS_DEPINA( const graph& g,
                            array< list<edge> >& mcb ) {
        array< d_int_set > mcb_temp;
        edge_num enumb( g );
        int w = MIN_CYCLE_BASIS_DEPINA( g, mcb_temp, enumb );
        MCB_output_basis( g, mcb_temp, enumb, mcb );
        return w;
}



// construct unique paths from v to u
void MCB_construct_unweighted_paths( const graph& G,
        node_array< node_array<edge> >& pred )
{
        // initialize
        node v,vis_t;
        node_array<int> dist(G,-1);
        list<node> vis;

        forall_nodes( v, G ) { 
                // init
                pred[v].init( G, nil );

                // run BFS
                forall( vis_t, vis ) dist[ vis_t ] = -1;
                vis = BFS( G, v, dist, pred[v] );
        }
}

 

// construct Horton's superset of MCB
void MCB_construct_superset( const graph& G,
                sortseq< two_tuple<int,int>, d_int_set >& super, 
                const edge_num& enumb )
{
    
    // initialize
    d_int_set C;
    node v,w,u,vz,ezs,ezt; 
    edge e,ez;

    int c = 0;
    node_array<int> degree(G,0);
    node_set touched( G );
    bool dege;

    int min;

    // construct paths
    node_array< node_array<edge> > pred( G );
    MCB_construct_unweighted_paths( G, pred );

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



            // create two paths
            int en;
            node y,x;
            
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
            two_tuple<int,int> tt( C.size(), c++ ); 
            super.insert( tt, C );
        }
    }
}


int MIN_CYCLE_BASIS_HYBRID( const graph& g,
                            array< d_int_set >& mcb,
                            array< d_int_set >& proof,
                            const edge_num& enumb
                            ) 
/*{\Mfunc $g$ is an undirected. The function computes a Minimum Cycle Basis $B$ of $g$, that 
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

        int N = enumb.dim_cycle_space();
        mcb.resize( N ); // set size of destination array
        proof.resize(N);



        array< d_int_set >& C = mcb;
        array< d_int_set >& S = proof;
        
        // initialize cycles C and initial sets S's
        for( int i = 0 ; i < N; i++ ) { S[i].insert( i ); }

   
        // construct superset
        sortseq< two_tuple<int,int> ,d_int_set> super( compare );
        MCB_construct_superset( g, super, enumb );
        
        int min = 0, ret = 0;
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

int MIN_CYCLE_BASIS_HYBRID( const graph& g,
                            array< d_int_set >& mcb,
                            const edge_num& enumb
                            )
/*{\Mfunc As above, but |proof| is not computed.
}*/
{
        array< d_int_set > proof_temp;
        return MIN_CYCLE_BASIS_HYBRID( g, mcb, proof_temp, enumb );
}

int MIN_CYCLE_BASIS_HYBRID( const graph& g,
                            array< list<edge> >& mcb,
                            array< list<edge> >& proof
                            )
/*{\Mfunc As above, but the minimum cycle basis is represented as an array of lists of edges.
}*/
{
        array< d_int_set > mcb_temp, proof_temp;
        edge_num enumb( g );
        int min = MIN_CYCLE_BASIS_HYBRID( g, mcb_temp, proof_temp, enumb );
        MCB_output_basis( g, mcb_temp, enumb, mcb );
        MCB_output_basis( g, mcb_temp, enumb, proof );
        return min;
}

int MIN_CYCLE_BASIS_HYBRID( const graph& g,
                            array< list<edge> >& mcb
                            )
/*{\Mfunc As above, but the minimum cycle basis is represented as an array of lists of 
edges and |proof| is not computed.}*/
{
        array< d_int_set > mcb_temp, proof_temp;
        edge_num enumb( g );
        int min = MIN_CYCLE_BASIS_HYBRID( g, mcb_temp, proof_temp, enumb );
        MCB_output_basis( g, mcb_temp, enumb, mcb );
        return min;
}



} // namespace mcb end

