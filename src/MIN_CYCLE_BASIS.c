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


#ifdef LEDA_GE_V5
#include <LEDA/graph/graph.h>
#include <LEDA/graph/node_set.h>
#include <LEDA/graph/edge_array.h>
#include <LEDA/graph/graph_misc.h>
#include <LEDA/graph/basic_graph_alg.h>
#include <LEDA/core/array.h>
#include <LEDA/core/list.h>
#include <LEDA/core/b_queue.h>
#include <LEDA/core/d_int_set.h>
#include <LEDA/core/sortseq.h>
#include <LEDA/core/tuple.h>
#include <LEDA/system/error.h>
#else
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
#endif
#include <LEP/mcb/edge_num.h>
#include <LEP/mcb/spvecgf2.h>
#include <LEP/mcb/SIGNED_GRAPH.h>
#include <LEP/mcb/min_cycle_basis.h>


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

// from spvecgf2 to edge_lists
void MCB_output_basis( const leda::graph& g,
                       const leda::array< mcb::spvecgf2 >& C,
                       const mcb::edge_num& enumb,
                       leda::array< leda::list< leda::edge > >& mcb )
{
    mcb.resize( C.size() );
    int el;
    for( int i = 0; i < C.size(); i++ ) { 
        mcb[i] = leda::list< leda::edge >();
        forall( el, C[i] ) { 
            mcb[i].append( enumb( el ) );
        }
    }
}

// from spvecgf2 to d_int_set
void MCB_output_basis( const leda::graph& g,
                       const leda::array< mcb::spvecgf2 >& C,
                       const mcb::edge_num& enumb,
                       leda::array<leda::d_int_set>& mcb )
{
    mcb.resize( C.size() );
    for( int i = 0; i < C.size(); i++ )
        mcb[i] = C[i].to_d_int_set();
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



#ifdef LEP_STATS
    float Tcycle = 0.0, Torthog = 0.0, Ttemp;
#endif

    array< d_int_set >& C = mcb;
    array< d_int_set >& S = proof;
    // initialize signed graph
    sgraph<int,leda::bin_heap> sg( g, enumb ) ; 


    // initialize cycles C and initial sets S's
    for( int i = 0 ; i < N; i++ ) { S[i].insert( i ); }
   
    int min = 0, ret = 0;
    // start computing cycles
    for( int k = 0; k < N; k++ ) { 
        
        #ifdef LEP_STATS
        leda::used_time( Ttemp );
        #endif
        #ifndef MCB_LEP_UNDIR_NO_EXCHANGE_HEURISTIC
        // choose the sparsest witness from the basis
        int minS = k;
        for( int r = k+1; r < N; r++ ) { 
            if ( S[r].size() < S[minS].size() )
                minS = r;
        }
        if ( minS != k ) {  // swap
            mcb::swap( S[k], S[minS] );
        }
        #endif

        #ifdef LEP_STATS
        Torthog += leda::used_time( Ttemp );
        #endif
        ret = sg.get_shortest_odd_cycle( S[k], C[k] ); 
        min+= ret;

        #ifdef LEP_STATS
        Tcycle += leda::used_time( Ttemp );
        #endif
        
        for( int l = k+1; l < N; l++ )   {
            // cycles are smaller than sets, thus it is better
            // to intersect C and S than S and C
            if ( (C[k].intersect(S[l])).size() %2 == 1 )   {
                S[ l ] %= S[k];
            }
        }

        #ifdef LEP_STATS
        Torthog += leda::used_time( Ttemp );
        #endif


    }

#ifdef LEP_STATS
    std::cout << "LEP_STATS: cycle computation time: " << Tcycle << std::endl;
    std::cout << "LEP_STATS: orthogonal base maintain time: " << Torthog << std::endl;
#endif

    return min;
}

// compute MCB with spvecgf2 output for 
// unweighted undirected graphs
int MIN_CYCLE_BASIS_DEPINA( const graph& g,
                            array< spvecgf2 >& mcb,
                            array< spvecgf2 >& proof,
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



#ifdef LEP_STATS
    float Tcycle = 0.0, Torthog = 0.0, Ttemp;
#endif

    array< spvecgf2 >& C = mcb;
    array< spvecgf2 >& S = proof;
    // initialize signed graph
    sgraph<int,leda::bin_heap> sg( g, enumb ) ; 


    // initialize cycles C and initial sets S's
    for( int i = 0 ; i < N; i++ ) { S[i].insert( i ); }
   
    int min = 0, ret = 0;
    // start computing cycles
    for( int k = 0; k < N; k++ ) { 
        
        #ifdef LEP_STATS
        leda::used_time( Ttemp );
        #endif
        #ifndef MCB_LEP_UNDIR_NO_EXCHANGE_HEURISTIC
        // choose the sparsest witness from the basis
        int minS = k;
        for( int r = k+1; r < N; r++ ) { 
            if ( S[r].size() < S[minS].size() )
                minS = r;
        }
        if ( minS != k ) {  // swap
            mcb::swap( S[k], S[minS] );
        }
        #endif

        #ifdef LEP_STATS
        Torthog += leda::used_time( Ttemp );
        #endif
        ret = sg.get_shortest_odd_cycle( S[k], C[k] ); 
        min+= ret;

        #ifdef LEP_STATS
        Tcycle += leda::used_time( Ttemp );
        #endif
        
        for( int l = k+1; l < N; l++ )   {
            // cycles are smaller than sets, thus it is better
            // to intersect C and S than S and C
            if ( (C[k].intersect(S[l])).size() %2 == 1 )   {
                S[ l ] %= S[k];
            }
        }

        #ifdef LEP_STATS
        Torthog += leda::used_time( Ttemp );
        #endif


    }

#ifdef LEP_STATS
    std::cout << "LEP_STATS: cycle computation time: " << Tcycle << std::endl;
    std::cout << "LEP_STATS: orthogonal base maintain time: " << Torthog << std::endl;
#endif

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

// compute MCB and proof with spvecgf2
// for unweighted undirected graphs
int MIN_CYCLE_BASIS_DEPINA( const graph& g,
                            array< spvecgf2 >& mcb,
                            const edge_num& enumb
                          ) { 
        array< spvecgf2 > proof;
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
        sortseq< two_tuple<int,int> ,d_int_set> super( leda::compare );
        MCB_construct_superset( g, super, enumb );
        
#ifdef LEP_STATS
        static int lep_stats_queries = 0;
        std::cout << "LEP_STATS: horton set size: " << super.size() << std::endl;
#endif
        
        int min = 0, ret = 0;
        leda::seq_item s;
        // start computing cycles
        for( int k = 0; k < N; k++ ) { 
                
                #ifdef LEP_STATS
                lep_stats_queries++;
                #endif

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

                #ifdef LEP_STATS
                    lep_stats_queries++;
                #endif
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

#ifdef LEP_STATS
        std::cout << "LEP_STATS: horton set queries: " << lep_stats_queries << std::endl;
#endif

        return min;
}

int MIN_CYCLE_BASIS_HYBRID( const graph& g,
                            array< d_int_set >& mcb,
                            const edge_num& enumb
                            )
{
        array< d_int_set > proof_temp;
        return MIN_CYCLE_BASIS_HYBRID( g, mcb, proof_temp, enumb );
}

int MIN_CYCLE_BASIS_HYBRID( const graph& g,
                            array< list<edge> >& mcb,
                            array< list<edge> >& proof
                            )
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
{
        array< d_int_set > mcb_temp, proof_temp;
        edge_num enumb( g );
        int min = MIN_CYCLE_BASIS_HYBRID( g, mcb_temp, proof_temp, enumb );
        MCB_output_basis( g, mcb_temp, enumb, mcb );
        return min;
}


void swap( d_int_set& a, d_int_set& b ) { 
    d_int_set tmp = a;
    a = b;
    b = tmp;
}



} // namespace mcb end

/* ex: set ts=8 sw=4 sts=4 noet: */


