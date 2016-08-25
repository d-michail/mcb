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

/*! \file mcb_approx.h
 *  \brief Algorithms for approximate MCB in undirected and directed graphs.
 *
 *  \sa mcb::edge_num mcb::spvecgf2 mcb::spvecfp
 */

#ifndef UMCB_APPROX_H
#define UMCB_APPROX_H

#include <LEP/mcb/config.h>
#include <LEP/mcb/umcb.h>
#include <LEP/mcb/spanner.h>
#include <LEP/mcb/ushortpath.h>
#include <LEP/mcb/dmcb.h>

#ifdef LEDA_GE_V5
#include <LEDA/graph/edge_map.h>
#include <LEDA/graph/node_map.h>
#else
#include <LEDA/edge_map.h>
#include <LEDA/node_map.h>
#endif

namespace mcb 
{ 

#if defined(LEDA_NAMESPACE)
    using leda::node;
    using leda::node_map;
    using leda::edge;
    using leda::edge_array;
    using leda::edge_map;
#endif

    
    template<typename W, class Container>
        class mcb_approx { 
            public:
                typedef W result_type;

                mcb_approx( const graph& g_, 
                        int k_,
                        array< Container >& mcb_, 
                        const mcb::edge_num& enumb_ )
                    : g(g_), k(k_), mcb(mcb_), enumb(enumb_), Tcycles(0.0), Tsubgraph(0.0)
                    { 
                    }

                virtual ~mcb_approx() {}

                W run() { 
                    checkGraphPreconditions();
                    checkEdgeLengthPreconditions();
                    initialize();

                    constructSpanner();

                    edge_num spanner_enumb( spanner );
                    array< Container > spanner_mcb;
                    constructSpannerMCB( spanner_enumb, spanner_mcb );

                    constructPartialMCBfromSpanner( spanner_enumb, spanner_mcb );
                    translateSpannerMCBToGraphPartialMCB( spanner_enumb, spanner_mcb );

                    return length;
                }

            private:

                void checkGraphPreconditions() { 
#if ! defined(LEDA_CHECKING_OFF)
                    if ( Is_Loopfree( g ) == false )
                        error_handler(999,"UMCB_APPROX: illegal graph (loops?)");
                    if ( k <= 0 )
                        error_handler(999,"UMCB_APPROX: illegal value of k, non-positive?");
#endif
                }

                virtual void checkEdgeLengthPreconditions() {}

                void initialize() { 
                    length = W();
                    N = enumb.dim_cycle_space();
                    mcb.resize( N );
                }

                virtual void constructSpanner() = 0;

                virtual void constructSpannerMCB( const edge_num& spanner_enumb, array< Container >& spanner_mcb ) = 0;

                virtual void constructPartialMCBfromSpanner( const edge_num& spanner_enumb, const array< Container >& spanner_mcb )  = 0;

                virtual void translateSpannerMCBToGraphPartialMCB( const edge_num& spanner_enumb, 
                                                           const array<Container>& spanner_mcb ) = 0;

            protected:
                const graph& g;
                int k;
                array< Container >& mcb;
                const mcb::edge_num& enumb;
                W length;
                int N;

                // spanner related
                graph spanner;
                edge_array<W> spanner_len;
                node_map<node> node_g_to_spanner;
                node_map<node> node_spanner_to_g;
                edge_map<edge> edge_g_to_spanner;
                edge_map<edge> edge_spanner_to_g;

                // statistics
                float Tcycles, Tsubgraph, Ttemp;
        };


    // undirected case
    template<typename W, class Container>
        class umcb_approx : public mcb_approx< W, Container > 
        { 
            public:
                typedef mcb_approx<W,Container> base_type;

                umcb_approx( const graph& g_, 
                        int k_,
                        array< Container >& mcb_, 
                        const mcb::edge_num& enumb_ )
                    : base_type( g_, k_, mcb_, enumb_ )
                    { 
                    }

                ~umcb_approx() {}

            private:

                virtual void constructSpannerMCB( const edge_num& spanner_enumb, array<Container>& spanner_mcb ) 
                { 
                    array< Container > spanner_proof;

#ifdef LEP_DEBUG_OUTPUT
                    std::cout << "Computing " << spanner_enumb.dim_cycle_space() << " cycles"; 
                    std::cout << " by the MCB of spanner..." << std::endl;
#endif

                    length += UMCB_SVA<W,Container>( spanner, spanner_len,
                            spanner_mcb, spanner_proof, spanner_enumb );
                }

                virtual void translateSpannerMCBToGraphPartialMCB( const edge_num& spanner_enumb, 
                                                           const array<Container>& spanner_mcb ) 
                { 
                    edge e;
                    int extracycles = spanner_enumb.dim_cycle_space();
                    for( int i = 0; i < extracycles; ++i ) 
                    {
                        mcb[ N - extracycles + i ] = Container();

                        int j = 0;
                        forall( j, spanner_mcb[ i ] ) 
                        { 
                            // translate to numbering of original graph 
                            // and add to cycle
                            e = edge_spanner_to_g[ spanner_enumb(j) ];
                            mcb[ N - extracycles + i ].insert( enumb( e ) );
                        }
                        // fix correct ordering
                        mcb[ N - extracycles + i ].sort();
                    }

#ifdef LEP_STATS
                    Tsubgraph += leda::used_time( Ttemp );
                    std::cout << "LEP_STATS: spanner cycles time: " << Tcycles << std::endl;
                    std::cout << "LEP_STATS: subgraph MCB time  : " << Tsubgraph << std::endl;
#endif
                }

                using base_type::length;
                using base_type::spanner;
                using base_type::spanner_len;
                using base_type::N;
                using base_type::mcb;
                using base_type::edge_spanner_to_g;
                using base_type::enumb;
        };

    // directed case
    template<typename W>
        class dmcb_approx : public mcb_approx<W,mcb::spvecfp> 
        { 
            public:
                typedef mcb_approx<W,mcb::spvecfp> base_type;

                dmcb_approx( const graph& g_, 
                        int k_,
                        array< mcb::spvecfp >& mcb_, 
                        const mcb::edge_num& enumb_,
                        double error_ 
                    )
                    : base_type( g_, k_, mcb_, enumb_ ), error(error_), prime(3), minimizeErrorProb(true)
                    { 
                    }

                dmcb_approx( const graph& g_, 
                        int k_,
                        array< mcb::spvecfp >& mcb_, 
                        const mcb::edge_num& enumb_,
                        ptype prime_
                    )
                    : base_type( g_, k_, mcb_, enumb_ ), error(0.375), prime(prime_), minimizeErrorProb(false)
                    { 
                    }

                ~dmcb_approx() {}

            protected:

                ptype getPrime( const edge_num& spanner_enumb, const array<mcb::spvecfp>& spanner_mcb ) 
                { 
                    return ( spanner_enumb.dim_cycle_space() > 0 ) ? spanner_mcb[0].pvalue() : 3;
                }

            private:

                virtual void constructSpannerMCB( const edge_num& spanner_enumb, array<mcb::spvecfp>& spanner_mcb ) 
                { 
                    array<spvecfp> spanner_proof;

#ifdef LEP_DEBUG_OUTPUT
                    std::cout << "Computing " << spanner_enumb.dim_cycle_space() << " cycles"; 
                    std::cout << " by the MCB of spanner..." << std::endl;
#endif

                    if ( minimizeErrorProb )
                        length += DMCB<W>( spanner, spanner_len,
                                spanner_mcb, spanner_proof, spanner_enumb, error );
                    else 
                        length += DMCB<W>( spanner, spanner_len,
                                spanner_mcb, spanner_proof, spanner_enumb, prime );

                }

                virtual void translateSpannerMCBToGraphPartialMCB( const edge_num& spanner_enumb, 
                                                           const array<spvecfp>& spanner_mcb ) 
                { 
                    edge e;
                    int extracycles = spanner_enumb.dim_cycle_space();
                    for( int i = 0; i < extracycles; ++i )
                    {
                        mcb[ N - extracycles + i] = mcb::spvecfp( getPrime( spanner_enumb, spanner_mcb ) );

                        leda::list_item it = spanner_mcb[i].first();
                        while( it != nil ) {
                            e = edge_spanner_to_g[ spanner_enumb( spanner_mcb[i].index( it ) ) ];
                            mcb[ N - extracycles + i ].append( enumb(e), spanner_mcb[i].inf( it ) );
                            it = spanner_mcb[i].succ( it );
                        }
                        mcb[ N - extracycles + i ].sort(); // fix ordering
                    }

#ifdef LEP_STATS
                    Tsubgraph += leda::used_time( Ttemp );
                    std::cout << "LEP_STATS: spanner cycles time: " << Tcycles << std::endl;
                    std::cout << "LEP_STATS: subgraph MCB time  : " << Tsubgraph << std::endl;
#endif
                }
        
                using base_type::length;
                using base_type::spanner;
                using base_type::spanner_len;
                using base_type::N;
                using base_type::mcb;
                using base_type::edge_spanner_to_g;
                using base_type::enumb;

            private:
                double error;
                ptype  prime;
                bool   minimizeErrorProb;
        };



                    
    // weighted undirected case
    template<typename W, class Container>
        class weighted_umcb_approx : public umcb_approx<W,Container>
        { 
            public:
                typedef umcb_approx<W,Container> base_type;

                weighted_umcb_approx( const graph& g_, 
                        const edge_array<W>& len_,
                        int k_,
                        array< Container >& mcb_, 
                        const mcb::edge_num& enumb_ )
                    : base_type( g_, k_, mcb_, enumb_ ), len(len_)
                    { 
                    }

                weighted_umcb_approx( const graph& g_, 
                        int k_,
                        array< Container >& mcb_, 
                        const mcb::edge_num& enumb_ )
                    : base_type( g_, k_, mcb_, enumb_ ), len(g_,1)
                    { 
                    }

                virtual ~weighted_umcb_approx() {}

            private:

                virtual void checkEdgeLengthPreconditions() { 
#if ! defined(LEDA_CHECKING_OFF)
                    edge e;
                    forall_edges( e , g ) { 
                        if ( len[e] < 0 ) 
                            error_handler(999,"UMCB_APPROX: illegal edge (negative weight?)");
                    }
#endif
                }

                virtual void constructSpanner() 
                { 
#ifdef LEP_STATS
                    leda::used_time( Ttemp );
#endif
                    mcb::detail::SPANNER( g, len, k, spanner, 
                            node_g_to_spanner, node_spanner_to_g,
                            edge_g_to_spanner, edge_spanner_to_g,
                            enumb);

                    // construct spanner edge lengths
                    edge e;
                    spanner_len.init( spanner );
                    forall_edges( e, spanner ) { 
                        spanner_len[ e ] = len[ edge_spanner_to_g[ e ] ];
                    }
                }

                virtual void constructPartialMCBfromSpanner( const edge_num& spanner_enumb, const array< Container >& spanner_mcb )
                { 
                    detail::ushortestpaths<W> usp( spanner, spanner_len );
                    int i = 0;
                    edge e, f;
                    forall_edges( e, g ) { 
                        if ( edge_g_to_spanner[e] == nil ) { 
                            mcb[i] = Container();

                            // compute shortest path on spanner
                            node spanner_s = node_g_to_spanner[ g.source(e) ];
                            node spanner_t = node_g_to_spanner[ g.target(e) ];
                            usp.compute_shortest_path( spanner_s, spanner_t  );

#if ! defined(LEDA_CHECKING_OFF)
                            assert( usp.is_reachable( spanner_t ) && usp.pred( spanner_s ) == nil );
#endif

                            // form cycle
                            W cycle_len = W();
                            node spanner_w = spanner_t;
                            while( usp.pred( spanner_w ) != nil ) { 
                                f = usp.pred( spanner_w );
                                mcb[i].insert( enumb( edge_spanner_to_g[ f ] ) );
                                cycle_len += len[ edge_spanner_to_g[ f ] ];
                                spanner_w = spanner.opposite( f, spanner_w );
                            }
                            mcb[i].insert( enumb( e ) );
                            cycle_len += len[ e ];
                            mcb[i].sort(); // fix correct ordering

                            // now update global cycles length
#if ! defined(LEDA_CHECKING_OFF)
                            if ( cycle_len < 0 ) 
                                error_handler(999,"UMCB_APPROX: computed cycle with negative length!");
#endif
                            length += cycle_len;

                            i++;
                        }
                    }

#ifdef LEP_STATS
                    Tcycles += leda::used_time( Ttemp );
#endif
#ifdef LEP_DEBUG_OUTPUT
                    std::cout << "Spanner has " << spanner.number_of_edges() << " edges..." << std::endl;
                    std::cout << "Computed " << i << " cycles fast..." << std::endl;
#endif
                }

            private:
                const edge_array<W> len;

                using base_type::g;
                using base_type::k;
                using base_type::length;
                using base_type::enumb;
                using base_type::mcb;
                using base_type::spanner;
                using base_type::spanner_len;
                using base_type::node_g_to_spanner;
                using base_type::node_spanner_to_g;
                using base_type::edge_g_to_spanner;
                using base_type::edge_spanner_to_g;
        };

    // weighted directed case
    template<typename W>
        class weighted_dmcb_approx : public dmcb_approx<W>
        { 
            public:
                typedef dmcb_approx<W> base_type;

                weighted_dmcb_approx( const graph& g_, 
                        const edge_array<W>& len_,
                        int k_,
                        array< spvecfp >& mcb_, 
                        const mcb::edge_num& enumb_, 
                        double error )
                    : base_type( g_, k_, mcb_, enumb_, error ), len(len_)
                    { 
                    }

                weighted_dmcb_approx( const graph& g_, 
                        int k_,
                        array< spvecfp >& mcb_, 
                        const mcb::edge_num& enumb_, 
                        double error )
                    : base_type( g_, k_, mcb_, enumb_, error ), len(g_,1)
                    { 
                    }

                weighted_dmcb_approx( const graph& g_, 
                        const edge_array<W>& len_,
                        int k_,
                        array< spvecfp >& mcb_, 
                        const mcb::edge_num& enumb_, 
                        ptype prime )
                    : base_type( g_, k_, mcb_, enumb_, prime ), len(len_)
                    { 
                    }

                weighted_dmcb_approx( const graph& g_, 
                        int k_,
                        array< spvecfp >& mcb_, 
                        const mcb::edge_num& enumb_, 
                        ptype prime )
                    : base_type( g_, k_, mcb_, enumb_, prime ), len(g_,1)
                    { 
                    }

                virtual ~weighted_dmcb_approx() {}

            private:

                virtual void checkEdgeLengthPreconditions() { 
#if ! defined(LEDA_CHECKING_OFF)
                    edge e;
                    forall_edges( e , g ) { 
                        if ( len[e] < 0 ) 
                            error_handler(999,"DMCB_APPROX: illegal edge (negative weight?)");
                    }
#endif
                }

                virtual void constructSpanner() 
                { 
#ifdef LEP_STATS
                    leda::used_time( Ttemp );
#endif
                    mcb::detail::SPANNER( g, len, k, spanner, 
                            node_g_to_spanner, node_spanner_to_g,
                            edge_g_to_spanner, edge_spanner_to_g,
                            enumb);

                    // construct spanner edge lengths
                    edge e;
                    spanner_len.init( spanner );
                    forall_edges( e, spanner ) { 
                        spanner_len[ e ] = len[ edge_spanner_to_g[ e ] ];
                    }
                }

                virtual void constructPartialMCBfromSpanner( const edge_num& spanner_enumb, const array< spvecfp >& spanner_mcb )
                { 
                    detail::ushortestpaths<W> usp( spanner, spanner_len );
                    int i = 0;
                    edge e, f;
                    forall_edges( e, g ) { 
                        if ( edge_g_to_spanner[e] == nil ) { 
                            mcb[i] = mcb::spvecfp( base_type::getPrime( spanner_enumb, spanner_mcb ) );

                            // compute shortest path on spanner
                            node spanner_s = node_g_to_spanner[ g.source(e) ];
                            node spanner_t = node_g_to_spanner[ g.target(e) ];
                            usp.compute_shortest_path( spanner_s, spanner_t  );

#if ! defined(LEDA_CHECKING_OFF)
                            assert( usp.is_reachable( spanner_t ) && usp.pred( spanner_s ) == nil );
#endif

                            // form cycle
                            node spanner_w = spanner_t;
                            while( usp.pred( spanner_w ) != nil ) {
                                f = usp.pred( spanner_w );
                                if ( spanner_w == spanner.source(f) ) {
                                    mcb[i].append( enumb( edge_spanner_to_g[ f ] ), 1 );
                                }
                                else {
                                    mcb[i].append( enumb( edge_spanner_to_g[ f ] ), -1 );
                                }
                                length += len[ edge_spanner_to_g[ f ] ];
                                spanner_w = spanner.opposite( f, spanner_w );
                            }
                            mcb[i].append( enumb( e ) , 1 );
                            length += len[ e ];
                            mcb[i].sort(); // fix correct ordering

                            i++;

                        }
                    }

#ifdef LEP_STATS
                    Tcycles += leda::used_time( Ttemp );
#endif
#ifdef LEP_DEBUG_OUTPUT
                    std::cout << "Spanner has " << spanner.number_of_edges() << " edges..." << std::endl;
                    std::cout << "Computed " << i << " cycles fast..." << std::endl;
#endif
                }

            private:
                const edge_array<W> len;

                using base_type::g;
                using base_type::k;
                using base_type::length;
                using base_type::enumb;
                using base_type::mcb;
                using base_type::spanner;
                using base_type::spanner_len;
                using base_type::node_g_to_spanner;
                using base_type::node_spanner_to_g;
                using base_type::edge_g_to_spanner;
                using base_type::edge_spanner_to_g;
        };



        // unweighted undirected case
        template<class Container>
        class unweighted_umcb_approx : public weighted_umcb_approx<int,Container> 
        { 
            public:
                typedef weighted_umcb_approx<int,Container> base_type;

                unweighted_umcb_approx( const graph& g_, 
                        int k_,
                        array< Container >& mcb_, 
                        const mcb::edge_num& enumb_ )
                    : base_type( g_, k_, mcb_, enumb_ )
                    { 
                    }

                ~unweighted_umcb_approx() {}

            private:

                virtual void checkEdgeLengthPreconditions() {}

                virtual void constructPartialMCBfromSpanner( const edge_num& spanner_enumb, const array< Container >& spanner_mcb ) 
                { 
                    detail::ubfs usp( spanner );
                    int i = 0;
                    edge e, f;
                    forall_edges( e, g ) { 
                        if ( edge_g_to_spanner[e] == nil ) { 
                            mcb[i] = Container();

                            // compute shortest path on spanner
                            node spanner_s = node_g_to_spanner[ g.source(e) ];
                            node spanner_t = node_g_to_spanner[ g.target(e) ];
                            usp.compute_shortest_path( spanner_s, spanner_t, 2*k-1 );

#if ! defined(LEDA_CHECKING_OFF)
                            assert( usp.is_reachable( spanner_t ) && usp.pred( spanner_s ) == nil );
#endif

                            // form cycle
                            node spanner_w = spanner_t;
                            while( usp.pred( spanner_w ) != nil ) { 
                                f = usp.pred( spanner_w );
                                mcb[i].insert( enumb( edge_spanner_to_g[ f ] ) );
                                ++length;
                                spanner_w = spanner.opposite( f, spanner_w );
                            }
                            mcb[i].insert( enumb( e ) );
                            ++length;
                            mcb[i].sort(); // fix correct ordering

                            ++i;
                        }
                    }

#ifdef LEP_STATS
                    Tcycles += leda::used_time( Ttemp );
#endif
#ifdef LEP_DEBUG_OUTPUT
                    std::cout << "Spanner has " << spanner.number_of_edges() << " edges..." << std::endl;
                    std::cout << "Computed " << i << " cycles fast..." << std::endl;
#endif
                }

                using base_type::g;
                using base_type::enumb;
                using base_type::mcb;
                using base_type::k;
                using base_type::length;
                using base_type::spanner;
                using base_type::edge_g_to_spanner;
                using base_type::edge_spanner_to_g;
                using base_type::node_g_to_spanner;
        };

    /*! \name Undirected Approximate Minimum Cycle Basis
     */

    //@{
    /*! \brief Compute an undirected approximate MCB of a weighted graph. 
     * 
     *  The function computes an \f$(2k-1)\f$-approximate Minimum Cycle
     *  Basis \f$B\f$ of a graph \f$g\f$.
     *  The basis is returned as an array of sparse vectors, spvecgf2, called mcb.<br>
     *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
     *  numbering is represented by enumb.<br>  
     *
     *  The function returns the weight of the approximate Minimum Cycle Basis or is undefined
     *  if there were any errors.<br> Even if the graph is directed this function computes an 
     *  approximate MCB of the underlying undirected graph.<br>
     *
     *  The running time is \f$O( m n^{1+1/k} + \min( m^3 + mn^2 \log n, n^{3+3/k}) )\f$ where \f$n\f$ are the number of nodes 
     *  of \f$g\f$, \f$m\f$ the number of edges and \f$k\f$ is an integer \f$\ge 1\f$.
     *
     *  \param g An graph.
     *  \param len A leda::edge_array for the edge lengths.
     *  \param k How much to approximate?
     *  \param mcb A leda::array of spvecgf2 to return the MCB.
     *  \param enumb An edge numbering.
     *  \return The length of the approximate MCB or undefined if some error occured.
     *  \pre g is loopfree.
     *  \pre len is non-negative
     *  \pre k must be an integer greater than zero
     *  \remark Care must be taken when the template parameter is instantiated with a data type
     *          which has rounding errors.
     *  \ingroup approxmcb
     */
    template<class W>
        W UMCB_APPROX( const graph& g,
                const edge_array<W>& len,
                const int k,
                array< mcb::spvecgf2 >& mcb,
                const mcb::edge_num& enumb
                ) 
        {
            weighted_umcb_approx<W,mcb::spvecgf2> tmp( g, len, k, mcb, enumb );
            return tmp.run(); 
        }


    /*! \brief Compute an undirected approximate MCB of a graph. 
     * 
     *  The function computes an \f$(2k-1)\f$-approximate Minimum Cycle
     *  Basis \f$B\f$ of a graph \f$g\f$.
     *  The basis is returned as an array of sparse vectors, spvecgf2, called mcb.<br>
     *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
     *  numbering is represented by enumb.<br>
     *
     *  The function returns the weight of the approximate Minimum Cycle Basis or is undefined
     *  if there were any errors.<br> Even if the graph is directed this function computes an 
     *  approximate MCB of the underlying undirected graph.<br>
     *
     *  The running time is \f$O( m n^{1+1/k} + \min( m^3 + mn^2 \log n, n^{3+3/k}) )\f$ where \f$n\f$ are the number of nodes 
     *  of \f$g\f$, \f$m\f$ the number of edges and \f$k\f$ is an integer \f$\ge 1\f$.
     *
     *  \param g An undirected graph.
     *  \param k How much to approximate?
     *  \param mcb A leda::array of spvecgf2 to return the MCB.
     *  \param enumb An edge numbering.
     *  \return The length of the approximate MCB or undefined if some error occured.
     *  \pre g is loopfree.
     *  \pre len is non-negative
     *  \pre k must be an integer greater than zero
     *  \remark Care must be taken when the template parameter is instantiated with a data type
     *          which has rounding errors.
     *  \ingroup approxmcb
     */
    int UMCB_APPROX( const graph& g,
            const int k,
            array< mcb::spvecgf2 >& mcb,
            const mcb::edge_num& enumb
            );
    //@}
    

    
    /*! \name Directed Approximate Minimum Cycle Basis
     */
    //@{
    /*! \brief Compute a directed approximate MCB of a weighted graph. 
     * 
     *  The function computes an \f$(2k-1)\f$-approximate Minimum Cycle
     *  Basis \f$B\f$ of a graph \f$g\f$.
     *  The basis is returned as an array of sparse vectors, spvecfp, called mcb.<br>
     *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
     *  numbering is represented by enumb.  
     *
     *  Since the algorithm is a randomized Monte-Carlo algorithm, the error argument 
     *  which should be less that 1 represents the acceptable error probability that the
     *  returned cycle basis is not a \f$(2k-1)\f$-approximate minimum cycle basis.
     *
     *  The function returns the weight of the approximate Minimum Cycle Basis or is undefined
     *  if there were any errors.<br>

     *  The running time is \f$O( mn^{1+1/k} + \min(m^3 + mn^2 \log n,n^{3+3/k}) )\f$ where \f$n\f$ are the number of 
     *  nodes of \f$g\f$, \f$m\f$ the number of edges 
     *  and \f$k\f$ is an integer \f$\ge 1\f$.
     *
     *  \param g An directed graph.
     *  \param len A leda::edge_array for the edge lengths.
     *  \param k How much to approximate?
     *  \param mcb A leda::array of spvecfp to return the approx MCB.
     *  \param enumb An edge numbering.
     *  \param error The error probability
     *  \return The length of the approximate MCB or undefined if some error occured.
     *  \pre g is loopfree.
     *  \pre len is non-negative
     *  \pre k must be an integer greater than zero
     *  \pre error is positive and less than one
     *  \remark Care must be taken when the template parameter is instantiated with a data type
     *          which has rounding errors.
     *  \ingroup approxmcb
     */
    template<class W>
        W DMCB_APPROX( const graph& g,
                const edge_array<W>& len,
                const int k,
                array< mcb::spvecfp >& mcb,
                const mcb::edge_num& enumb,
                double error = 0.375
                )
    { 
            weighted_dmcb_approx<W> tmp( g, len, k, mcb, enumb, error );
            return tmp.run(); 
    }

    /*! \brief Compute a directed approximate MCB of a weighted graph in \f$\mathcal{F}_p\f$.
     *         Note that the minimum cycle basis in \f$\mathcal{F}_p\f$ might not be the minimum
     *         cycle basis of the graph.
     * 
     *  The basis is returned as an array of sparse vectors, spvecfp, called mcb.<br>
     *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
     *  numbering is represented by enumb.  
     *
     *  The function returns the weight of the possibly approximate Minimum Cycle Basis or is undefined
     *  if there were any errors.<br>

     *  The running time is \f$O( mn^{1+1/k} + \min(m^3 + mn^2 \log n,n^{3+3/k}) )\f$ where \f$n\f$ are the number of 
     *  nodes of \f$g\f$, \f$m\f$ the number of edges 
     *  and \f$k\f$ is an integer \f$\ge 1\f$.
     *
     *  \param g An directed graph.
     *  \param len A leda::edge_array for the edge lengths.
     *  \param k How much to approximate?
     *  \param mcb A leda::array of spvecfp to return the approx MCB.
     *  \param enumb An edge numbering.
     *  \param prime A leda::integer prime number in order to do the computation in \f$\mathcal{F}_p\f$.
     *  \return The length of the approximate MCB or undefined if some error occured.
     *  \pre g is loopfree.
     *  \pre len is non-negative
     *  \pre k must be an integer greater than zero
     *  \pre prime is a prime number > 2
     *  \remark Care must be taken when the template parameter is instantiated with a data type
     *          which has rounding errors.
     *  \ingroup approxmcb
     */
    template<class W>
        W DMCB_APPROX( const graph& g,
                const edge_array<W>& len,
                const int k,
                array< mcb::spvecfp >& mcb,
                const mcb::edge_num& enumb,
                mcb::ptype prime
                )
    { 
            weighted_dmcb_approx<W> tmp( g, len, k, mcb, enumb, prime );
            return tmp.run(); 
    }


    //@}


    /*! \name Undirected Minimum Cycle Basis
     *  The functions below are the more general implementation for undirected graphs. They 
     *  also support multigraphs. As an underlying implementation they use the Support Vector
     *  approach (mcb::UMCB_SVA).
     *  They should be the first choice of use unless some special requirements exist.
     */
    //@{

    /*! \brief Compute an MCB of an undirected graph (possible a multigraph) using the most 
     *         general implementation of this library.
     * 
     *  The function computes a Minimum Cycle
     *  Basis \f$B\f$ of a graph \f$g\f$, that is a cycle basis of \f$g\f$ with the smallest 
     *  length (sum of lengths of cycles). 
     *  The basis is returned as an array of mcb:spvecgf2 objects.<br>
     *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
     *  numbering is represented by enumb. 
     *
     *  The function returns the weight of the Minimum Cycle Basis or is undefined
     *  if there were any errors.<br>
     *  The running time is \f$O( m^3 )\f$ where \f$m\f$ are the number of edges of \f$g\f$.
     *
     *  \param g An undirected graph.
     *  \param len The edge lengths function.
     *  \param mcb A leda::array of mcb::spvecgf2 to return the MCB.
     *  \param enumb An edge numbering.
     *  \return The length of the MCB or undefined if some error occured.
     *  \pre g is undirected
     *  \remark Care must be taken when the template parameter is instantiated with a data type
     *          which has rounding errors.
     *  \ingroup exactmcb
     */
    template<class W>
        W UMCB( const graph& g,
                const edge_array<W>& len,
                array< mcb::spvecgf2 >& mcb,
                const mcb::edge_num& enumb
                ) 
        {
            return UMCB_APPROX<W>( g, len, 1, mcb, enumb );
        }

    /*! \brief Compute an MCB of an undirected graph (possible a multigraph) using the most 
     *         general implementation of this library.
     * 
     *  The function computes a Minimum Cycle
     *  Basis \f$B\f$ of a graph \f$g\f$, that is a cycle basis of \f$g\f$ with the smallest 
     *  length (sum of lengths of cycles). 
     *  The basis is returned as an array of mcb:spvecgf2 objects.<br>
     *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
     *  numbering is represented by enumb. 
     *
     *  The function returns the weight of the Minimum Cycle Basis or is undefined
     *  if there were any errors.<br>
     *  The running time is \f$O( m^3 )\f$ where \f$m\f$ are the number of edges of \f$g\f$.
     *
     *  \param g An undirected graph.
     *  \param mcb A leda::array of mcb::spvecgf2 to return the MCB.
     *  \param enumb An edge numbering.
     *  \return The length of the MCB or undefined if some error occured.
     *  \pre g is undirected
     *  \remark Care must be taken when the template parameter is instantiated with a data type
     *          which has rounding errors.
     *  \ingroup exactmcb
     */
    int UMCB( const graph& g,
            array< mcb::spvecgf2 >& mcb,
            const mcb::edge_num& enumb
            );
    //@}

}

#endif  // UMCB_APPROX_H

/* ex: set ts=4 sw=4 sts=4 et: */


