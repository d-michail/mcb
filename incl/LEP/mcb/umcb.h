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

/*! \file umcb.h
 *  \brief Algorithms for undirected MCB.
 *
 *  Given an undirected graph \f$G(V,E)\f$ and a non-negative length function on the edges
 *  \f$w: E \mapsto \mathcal{R}_{\ge 0}\f$, a minimum cycle basis is a set of cycles
 *  which can generate the cycle space and at the same time has minimum total length.
 *
 *  Each cycle of the graph is assumed to be a 0-1 vector indexed on the edge set, and
 *  operations between cycles is performed in GF(2). The length of a cycle basis is 
 *  the sum of the length of its cycles and the length of a cycle is the sum of the 
 *  length of its edges.
 *
 *  The solution of a minimum cycle basis problem can be in the following two forms.
 *  - A pair (mcb, proof) where both are arrays of mcb::spvecgf2, array< mcb::spvecgf2 >.
 *    Each mcb::spvecgf2 represents a cycle or some edge set. Each edge of the graph
 *    is represented by a unique number provided by an edge numbering, mcb::edge_num.
 *  - A pair (mcb, proof) where both are arrays of compressed integer sets, array< d_int_set >.
 *      Each integer sets represents a cycle. In these integer sets, each edge of the graph 
 *      is represented by a unique number provided by the edge numbering, mcb::edge_num.
 *
 *  Most functions of this section are templates functions. The template parameter W can be instantiated
 *  with any number type. Attention must be paid in order to avoid overflow of values.
 *
 *  The whole package is protected using a namespace called "mcb", and therefore using 
 *  anything requires <b>mcb::xx</b> or the <b>using namespace mcb</b> directive.
 *
 *  \sa mcb::edge_num mcb::spvecgf2
 */

#ifndef UMCB_H
#define UMCB_H

#include <LEP/mcb/config.h>

#ifdef LEDA_GE_V5
#include <LEDA/graph/graph.h>
#include <LEDA/graph/edge_array.h>
#include <LEDA/graph/node_array.h>
#include <LEDA/core/d_int_set.h>
#include <LEDA/core/array.h>
#include <LEDA/core/list.h>
#else
#include <LEDA/graph.h>
#include <LEDA/d_int_set.h>
#include <LEDA/edge_array.h>
#include <LEDA/node_array.h>
#include <LEDA/array.h>
#include <LEDA/list.h>
#endif

#include <LEP/mcb/spvecgf2.h>
#include <LEP/mcb/signed.h>
#include <LEP/mcb/superset.h>
#include <LEP/mcb/sptrees.h>
#include <LEP/mcb/transform.h>
#include <LEP/mcb/verify.h>

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
#endif

    template<typename W, class Container>
    class SupportMCB
    {
        public:
            typedef W result_type;

            SupportMCB( const graph& g_, 
                        array< Container >& mcb_, 
                        array< Container >& proof_,
                        const mcb::edge_num& enumb_ 
                        ) 
                : g(g_), C(mcb_), S(proof_), enumb(enumb_), N(enumb_.dim_cycle_space())
            {
#if ! defined(LEDA_CHECKING_OFF)
                if ( Is_Undirected_Simple( g ) == false )
                    error_handler(999,"MIN_CYCLE_BASIS: illegal graph (parallel,anti-parallel edges or loops?)");
#endif
                C.resize(N);
                S.resize(N);
            }

            virtual ~SupportMCB() {}

            W run() { 
                checkPreconditions();

#ifdef LEP_STATS
                float Tcycle = 0.0, Torthog = 0.0, Ttemp;
#endif

                initializeSupportVectors();
                
                W min = W();
                for( int k = 0; k < N; ++k ) {
#ifdef LEP_STATS
                    leda::used_time( Ttemp );
#endif
                    chooseSparsestSupportHeuristic( k );

#ifdef LEP_STATS
                    Torthog += leda::used_time( Ttemp );
#endif

                    min += computeShortestOddCycle( k );

#ifdef LEP_STATS
                    Tcycle += leda::used_time( Ttemp );
#endif
                    updateSupportVectors( k );

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

        private:

            virtual void checkPreconditions() = 0;

            void initializeSupportVectors() {
                for( int i = 0; i < N; ++i ) { 
                    S[i].clear(); 
                    S[i].insert(i); 
                } 
            }

            void chooseSparsestSupportHeuristic( int k ) { 
#ifndef MCB_LEP_UNDIR_NO_EXCHANGE_HEURISTIC
                int minS = k;
                for( int r = k+1; r < N; ++r ) {
                    if ( S[r].size() < S[minS].size() )
                        minS = r;
                }
                if ( minS != k ) {  // swap
                    std::swap( S[k], S[minS] );
                }
#endif
            }

            virtual W computeShortestOddCycle( int k ) = 0;

            void updateSupportVectors( int k ) { 
                for( int l = k+1; l < N; ++l )   {
                    if ( (C[k].intersect(S[l])).size() % 2 == 1 )   {
                        S[ l ] %= S[k];
                    }
                }
            }

        protected:
            const graph& g;
            array<Container>& C;
            array<Container>& S;
            const mcb::edge_num& enumb; 
            int N; 
    };

    template<typename W, class Container>
    class WeightedSupportMCB: public SupportMCB< W, Container>
    {
        public:
            typedef SupportMCB< W, Container> base_type;

            WeightedSupportMCB( const graph& g_, 
		                const edge_array<W>& len_,
                        array< Container >& mcb_, 
                        array< Container >& proof_,
                        const mcb::edge_num& enumb_ ) 
                : base_type( g_, mcb_, proof_, enumb_ ), len(len_)
            {
            }

        private:

            void checkPreconditions() { 
#if ! defined(LEDA_CHECKING_OFF)
                edge e;
                forall_edges( e , g ) {
                    if ( len[e] < 0 )
                        error_handler(999,"MIN_CYCLE_BASIS: illegal edge (negative weight)");
                }
#endif
            }

        protected:
            const edge_array<W>& len;
            using base_type::g;
    };

    template<typename W, class Container>
    class WeightedSignedSupportMCB: public WeightedSupportMCB< W, Container>
    {
        public:
            typedef WeightedSupportMCB< W, Container> base_type;

            WeightedSignedSupportMCB( const graph& g_, 
		                const edge_array<W>& len_,
                        array< Container >& mcb_, 
                        array< Container >& proof_,
                        const mcb::edge_num& enumb_ ) 
                : base_type( g_, len_, mcb_, proof_, enumb_ ), sg(g_, len_, enumb_)
            {
            }

        private:

            W computeShortestOddCycle( int k ) { 
                return sg.get_shortest_odd_cycle( S[k], C[k] );
            }

        private:
            detail::WeightedSignedGraph<W,leda::bin_heap> sg;
            using base_type::C;
            using base_type::S;
    };

    template<class Container>
    class UnweightedSignedSupportMCB: public SupportMCB< int, Container >
    {
        public:
            typedef SupportMCB< int, Container> base_type;

            UnweightedSignedSupportMCB( const graph& g_, 
                        array< Container >& mcb_, 
                        array< Container >& proof_,
                        const mcb::edge_num& enumb_ 
                        ) 
                : base_type( g_, mcb_, proof_, enumb_ ), sg( g_, enumb_ )
            {
            }

            ~UnweightedSignedSupportMCB() {}

        private:
            int computeShortestOddCycle( int k ) { 
                return sg.get_shortest_odd_cycle( S[k], C[k] );
            }

            void checkPreconditions() {}

        private:
            detail::UnweightedSignedGraph sg;
            using base_type::C;
            using base_type::S;
    };

    template<typename W, class Container>
    class WeightedHortonSupportMCB: public WeightedSupportMCB<W, Container>
    {
        public:
            typedef WeightedSupportMCB<W, Container> base_type;

            WeightedHortonSupportMCB( const graph& g_, 
		                const edge_array<W>& len_,
                        array< Container >& mcb_, 
                        array< Container >& proof_,
                        const mcb::edge_num& enumb_ ) 
                : base_type( g_, len_, mcb_, proof_, enumb_ ), HS( g_, len_, enumb_ )
            {
            }

        private:

            W computeShortestOddCycle( int k ) { 
                return HS.get_shortest_odd_cycle( S[k], C[k] );
            }

            HortonSuperset<W> HS;
            using base_type::C;
            using base_type::S;
    };

    template<typename W, class Container>
    class WeightedSPTreesSupportMCB: public WeightedSupportMCB<W, Container>
    {
        public:
            typedef WeightedSupportMCB<W, Container> base_type;

            WeightedSPTreesSupportMCB( const graph& g_, 
		                const edge_array<W>& len_,
                        array< Container >& mcb_, 
                        array< Container >& proof_,
                        const mcb::edge_num& enumb_ ) 
                : base_type( g_, len_, mcb_, proof_, enumb_ ), uhst( g_, len_, enumb_ )
            {
            }

        private:

            W computeShortestOddCycle( int k ) { 
                uhst.updateTreeLabels( S[k] );
                return uhst.getShortestOddCycle( S[k], C[k] );
            }

            UndirectedHortonSupersetTrees<W> uhst;
            using base_type::C;
            using base_type::S;
    };



    /*! \name Undirected Minimum Cycle Basis
     *  These functions implement the three main approaches for computing a minimum cycle basis.
     *  All these approaches are based on the <b>Support Vector Approach</b>. Their differences are
     *  on the way of finding the shortest non-orthogonal cycle: 
     *  - mcb::UMCB_SVA finds the cycles by maintaining a signed graph
     *  - mcb::UMCB_HYBRID maintains a list of candidate cycles which is guarantied to contain 
     *    an MCB, 
     *  - mcb::UMCB_FH maintains the same list as above but in a more elaborate way in order to be 
     *    able to compute faster the shortest non-orthogonal cycle.
     *
     *  In case you have no special preference try the mcb::UMCB function which uses the signed graph 
     *  implementation and supports multigraphs.
     *
     *  The fastest algorithm is mcb::UMCB_SVA.
     */
    //@{

    /*! \brief Compute a MCB of an undirected graph using the Support Vector Approach
     *         (de Pina's PhD thesis) algorithm.
     * 
     *  The function computes a Minimum Cycle
     *  Basis \f$B\f$ of a graph \f$g\f$, that is a cycle basis of \f$g\f$ with the smallest 
     *  length (sum of lengths of cycles). It accepts one template parameter which is the 
     *  container to use for recording the cycles. Currently
     *  two options are accepted, leda::d_int_set and mcb::spvecgf2.
     *  The basis is returned as an array of Container objects.<br>
     *
     *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
     *  numbering is represented by enumb. The function also returns a certificate of optimality of 
     *  the minimum cycle basis. More precisely a set of linearly independent vectors for 
     *  which cycle \f$i\f$ is the shortest cycle in \f$g\f$ with odd intersection with the 
     *  proof vector \f$i\f$.<br>
     *
     *  The function returns the weight of the Minimum Cycle Basis or is undefined
     *  if there were any errors.<br>
     *  The running time is \f$O( m^3 )\f$ where \f$m\f$ are the number of edges of \f$g\f$.
     *
     *  \param g An undirected graph.
     *  \param mcb A leda::array of Container to return the MCB.
     *  \param proof A leda::array of Container to return the proof.
     *  \param enumb An edge numbering.
     *  \return The length of the MCB or undefined if some error occured.
     *  \pre g is undirected, simple and loopfree.
     *  \ingroup exactmcb
     */
    template<class Container>
	int UMCB_SVA( const graph& g,
		array< Container >& mcb,
		array< Container >& proof,
		const mcb::edge_num& enumb
		) 
	{
        UnweightedSignedSupportMCB<Container> tmp( g, mcb, proof, enumb );
        return tmp.run();
	}

    /*! \brief Compute a MCB of an undirected weighted graph using the Support Vector Approach
     *         (de Pina's PhD thesis) algorithm.
     *
     *  The function computes a Minimum Cycle
     *  Basis \f$B\f$ of a graph \f$g\f$, that is a cycle basis of \f$g\f$ with the smallest 
     *  length (sum of lengths of cycles). It accepts one template parameters which is the 
     *  container to use for recording the cycles. Currently
     *  two options are accepted, leda::d_int_set and mcb::spvecgf2.
     *  The basis is returned as an array of Container objects.<br>
     *
     *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
     *  numbering is represented by enumb.<br>
     *
     *  The function returns the weight of the Minimum Cycle Basis or is undefined
     *  if there were any errors.<br>
     *  The running time is \f$O( m^3 )\f$ where \f$m\f$ are the number of edges of \f$g\f$.
     *
     *  \param g An undirected graph.
     *  \param mcb A leda::array of Container to return the MCB.
     *  \param enumb An edge numbering.
     *  \return The length of the MCB or undefined if some error occured.
     *  \pre g is undirected, simple and loopfree.
     *  \ingroup exactmcb
     */
    template<class Container>
    int UMCB_SVA( const graph& g, 
        array< Container >& mcb, 
        const mcb::edge_num& enumb
        )
    {
        array< Container > proof;
        return UMCB_SVA( g, mcb, proof, enumb );
    }

    /*! \brief Compute a MCB of an undirected weighted graph using the Support Vector Approach
     *         (de Pina's PhD thesis) algorithm.
     * 
     *  The function computes a Minimum Cycle
     *  Basis \f$B\f$ of a graph \f$g\f$, that is a cycle basis of \f$g\f$ with the smallest 
     *  length (sum of lengths of cycles). It accepts two template parameters. The first is
     *  length type and the second is the container to use for recording the cycles. Currently
     *  two options are accepted, leda::d_int_set and mcb::spvecgf2.
     *  The basis is returned as an array of Container objects.<br>
     *
     *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
     *  numbering is represented by enumb. The function also returns a certificate of optimality of 
     *  the minimum cycle basis. More precisely a set of linearly independent vectors for 
     *  which cycle \f$i\f$ is the shortest cycle in \f$g\f$ with odd intersection with the 
     *  proof vector \f$i\f$.<br>
     *
     *  The function returns the weight of the Minimum Cycle Basis or is undefined
     *  if there were any errors.<br>
     *  The running time is \f$O( m^3 )\f$ where \f$m\f$ are the number of edges of \f$g\f$.
     *
     *  \param g An undirected graph.
     *  \param len A leda::edge_array for the edge lengths.
     *  \param mcb A leda::array of Container to return the MCB.
     *  \param proof A leda::array of Container to return the proof.
     *  \param enumb An edge numbering.
     *  \return The length of the MCB or undefined if some error occured.
     *  \pre g is undirected, simple and loopfree.
     *  \pre len is non-negative
     *  \remark Care must be taken when the template parameter is instantiated with a data type
     *          which has rounding errors.
     *  \ingroup exactmcb
     */
    template<class W, class Container>
	W UMCB_SVA( const graph& g,
		const edge_array<W>& len,
		array< Container >& mcb,
		array< Container >& proof,
		const mcb::edge_num& enumb
		) 
	{ 
        WeightedSignedSupportMCB<W,Container> tmp( g, len, mcb, proof, enumb );
        return tmp.run();
    }

    /*! \brief Compute a MCB of an undirected weighted graph using the Support Vector Approach
     *         (de Pina's PhD thesis) algorithm.
     * 
     *  The function computes a Minimum Cycle
     *  Basis \f$B\f$ of a graph \f$g\f$, that is a cycle basis of \f$g\f$ with the smallest 
     *  length (sum of lengths of cycles). It accepts two template parameters. The first is
     *  length type and the second is the container to use for recording the cycles. Currently
     *  two options are accepted, leda::d_int_set and mcb::spvecgf2.
     *  The basis is returned as an array of Container objects.<br>
     *
     *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
     *  numbering is represented by enumb. 
     *
     *  The function returns the weight of the Minimum Cycle Basis or is undefined
     *  if there were any errors.<br>
     *  The running time is \f$O( m^3 )\f$ where \f$m\f$ are the number of edges of \f$g\f$.
     *
     *  \param g An undirected graph.
     *  \param len A leda::edge_array for the edge lengths.
     *  \param mcb A leda::array of Container to return the MCB.
     *  \param enumb An edge numbering.
     *  \return The length of the MCB or undefined if some error occured.
     *  \pre g is undirected, simple and loopfree.
     *  \pre len is non-negative
     *  \remark Care must be taken when the template parameter is instantiated with a data type
     *          which has rounding errors.
     *  \ingroup exactmcb
     */
    template<class W, class Container>
	W UMCB_SVA( const graph& g,
		const edge_array<W>& len,
		array< Container >& mcb,
		const mcb::edge_num& enumb
		) 
	{ 
		array< Container > proof;
        WeightedSignedSupportMCB<W,Container> tmp( g, len, mcb, proof, enumb );
        return tmp.run();
    }

    /*! \brief Compute a minimum cycle basis of an undirected graph using a hybrid algorithm.
     *
     *  The function computes a minimum cycle basis of an undirected
     *  graph \f$g\f$, that is a cycle basis of \f$g\f$ with the smallest length 
     *  (sum of lengths of cycles). The 
     *  basis is returned as an array of dynamic integer sets, d_int_set, called mcb. 
     *
     *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
     *  numbering is represented by enumb. 
     *
     *  The function also returns a certificate of optimality of 
     *  the minimum cycle basis. More precisely a set of linearly independent vectors for 
     *  which cycle \f$i\f$ is the shortest cycle in \f$g\f$ with odd intersection with the 
     *  proof vector \f$i\f$. 
     *
     *  The function returns the weight of the Minimum Cycle Basis or is undefined
     *  if there were any errors.<br> 
     *  The running time is \f$O( m^2 n^2 )\f$ where \f$m\f$ are the number of edges of \f$g\f$ 
     *  and \f$n\f$ the number of vertices. 
     *  \param g An undirected graph.
     *  \param mcb A leda::array of leda::d_int_set to return the MCB.
     *  \param proof A leda::array of leda::d_int_set to return the proof.
     *  \param enumb An edge numbering.
     *  \return The length of the MCB or undefined if some error occured.
     *  \pre g is undirected, simple and loopfree 
     *  \ingroup exactmcb
     */
    extern int UMCB_HYBRID( const leda::graph& g,
            leda::array< leda::d_int_set >& mcb,
            leda::array< leda::d_int_set >& proof,
            const mcb::edge_num& enumb
            );

    /*! \brief Compute a minimum cycle basis of an undirected graph using a hybrid algorithm.
     *
     *  The function computes a minimum cycle basis of an undirected
     *  graph \f$g\f$, that is a cycle basis of \f$g\f$ with the smallest length 
     *  (sum of lengths of cycles). The 
     *  basis is returned as an array of dynamic integer sets, d_int_set, called mcb. 
     *
     *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
     *  numbering is represented by enumb. 
     *
     *  The function returns the weight of the Minimum Cycle Basis or is undefined
     *  if there were any errors.<br> 
     *  The running time is \f$O( m^2 n^2 )\f$ where \f$m\f$ are the number of edges of \f$g\f$ 
     *  and \f$n\f$ the number of vertices. 
     *  \param g An undirected graph.
     *  \param mcb A leda::array of leda::d_int_set to return the MCB.
     *  \param enumb An edge numbering.
     *  \return The length of the MCB or undefined if some error occured.
     *  \pre g is undirected, simple and loopfree 
     *  \ingroup exactmcb
     */
    extern int UMCB_HYBRID( const leda::graph& g,
            leda::array< leda::d_int_set >& mcb,
            const mcb::edge_num& enumb
            );


    /*! \brief Compute a minimum cycle basis of a weighted undirected graph using a hybrid algorithm.
     *
     *  The function computes a minimum cycle basis of an undirected weighted 
     *  graph \f$g\f$, that is a cycle basis of \f$g\f$ with the smallest length 
     *  (sum of lengths of cycles). The 
     *  basis is returned as an array of dynamic integer sets, d_int_set, called mcb. 
     *
     *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
     *  numbering is represented by enumb. 
     *
     *  The function also returns a certificate of optimality of 
     *  the minimum cycle basis. More precisely a set of linearly independent vectors for 
     *  which cycle \f$i\f$ is the shortest cycle in \f$g\f$ with odd intersection with the 
     *  proof vector \f$i\f$. 
     *
     *  The function returns the weight of the Minimum Cycle Basis or is undefined
     *  if there were any errors.<br> 
     *  The running time is \f$O( m^2 n^2 )\f$ where \f$m\f$ are the number of edges of \f$g\f$ 
     *  and \f$n\f$ the number of vertices. 
     *  \param g An undirected graph.
     *  \param len A leda::edge_array for the edge lengths.
     *  \param mcb A leda::array of leda::d_int_set to return the MCB.
     *  \param proof A leda::array of leda::d_int_set to return the proof.
     *  \param enumb An edge numbering.
     *  \return The length of the MCB or undefined if some error occured.
     *  \pre g is undirected, simple and loopfree 
     *  \pre len is non-negative 
     *  \ingroup exactmcb
     */
    template<class W>
        W UMCB_HYBRID( const graph& g,
                const edge_array<W>& len,
                array< d_int_set >& mcb,
                array< d_int_set >& proof,
                const mcb::edge_num& enumb
                ) 
        { 
            WeightedHortonSupportMCB<W, d_int_set> tmp( g, len, mcb, proof, enumb );  
            return tmp.run();
        }


    /*! \brief Compute a minimum cycle basis of a weighted undirected graph using a hybrid algorithm.
     *
     *  The function computes a minimum cycle basis of an undirected weighted 
     *  graph \f$g\f$, that is a cycle basis of \f$g\f$ with the smallest length 
     *  (sum of lengths of cycles). The 
     *  basis is returned as an array of dynamic integer sets, d_int_set, called mcb. 
     *
     *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
     *  numbering is represented by enumb. 
     *
     *  The function returns the weight of the Minimum Cycle Basis or is undefined
     *  if there were any errors.<br> 
     *  The running time is \f$O( m^2 n^2 )\f$ where \f$m\f$ are the number of edges of \f$g\f$ 
     *  and \f$n\f$ the number of vertices. 
     *  \param g An undirected graph.
     *  \param len A leda::edge_array for the edge lengths.
     *  \param mcb A leda::array of leda::d_int_set to return the MCB.
     *  \param enumb An edge numbering.
     *  \return The length of the MCB or undefined if some error occured.
     *  \pre g is undirected, simple and loopfree 
     *  \pre len is non-negative 
     *  \ingroup exactmcb
     */
    template<class W>
        W UMCB_HYBRID( const graph& g,
                const edge_array<W>& len,
                array< d_int_set >& mcb,
                const mcb::edge_num& enumb
                )
        {
            array< d_int_set > proof_temp;
            return UMCB_HYBRID( g, len, mcb, proof_temp, enumb );
        }

    /*! \brief Compute a MCB of an undirected weighted graph using a Fast variant of the 
     *         Hybrid algorithm.
     * 
     *  The function computes a Minimum Cycle Basis \f$B\f$ of a graph \f$g\f$, that is a cycle basis 
     *  of \f$g\f$ with the smallest 
     *  length (sum of lengths of cycles). It accepts the length type as a template parameter.
     *  The basis is returned as an array of mcb::spvecgf2 objects.<br>
     *
     *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
     *  numbering is represented by enumb. The function also returns a certificate of optimality of 
     *  the minimum cycle basis. More precisely a set of linearly independent vectors for 
     *  which cycle \f$i\f$ is the shortest cycle in \f$g\f$ with odd intersection with the 
     *  proof vector \f$i\f$.<br>
     *
     *  The function returns the weight of the Minimum Cycle Basis or is undefined
     *  if there were any errors.<br>
     *  The running time is \f$O( m^3 )\f$ where \f$m\f$ are the number of edges of \f$g\f$.
     *
     *  \param g An undirected graph.
     *  \param len A leda::edge_array for the edge lengths.
     *  \param mcb A leda::array of mcb::spvecgf2 to return the MCB.
     *  \param proof A leda::array of mcb::spvecgf2 to return the proof.
     *  \param enumb An edge numbering.
     *  \return The length of the MCB or undefined if some error occured.
     *  \pre g is undirected, simple and loopfree.
     *  \pre len is non-negative
     *  \remark Care must be taken when the template parameter is instantiated with a data type
     *          which has rounding errors.
     *  \ingroup exactmcb
     */
    template<class W>
    W UMCB_FH( const graph& g, 
                const edge_array<W>& len,
                array< mcb::spvecgf2 >& mcb, 
                array< mcb::spvecgf2 >& proof, 
                const mcb::edge_num& enumb ) 
    { 
        WeightedSPTreesSupportMCB<W,mcb::spvecgf2> tmp( g, len, mcb, proof, enumb );
        return tmp.run();
    }

    /*! \brief Compute a MCB of an undirected weighted graph using a Fast variant of the 
     *         Hybrid algorithm.
     * 
     *  The function computes a Minimum Cycle Basis \f$B\f$ of a graph \f$g\f$, that is a cycle basis 
     *  of \f$g\f$ with the smallest 
     *  length (sum of lengths of cycles). It accepts the length type as a template parameter.
     *  The basis is returned as an array of mcb::spvecgf2 objects.<br>
     *
     *  Every edge is represented by some integer, by a fixed and arbitrary numbering. This
     *  numbering is represented by enumb. 
     *
     *  The function returns the weight of the Minimum Cycle Basis or is undefined
     *  if there were any errors.<br>
     *  The running time is \f$O( m^3 )\f$ where \f$m\f$ are the number of edges of \f$g\f$.
     *
     *  \param g An undirected graph.
     *  \param len A leda::edge_array for the edge lengths.
     *  \param mcb A leda::array of mcb::spvecgf2 to return the MCB.
     *  \param enumb An edge numbering.
     *  \return The length of the MCB or undefined if some error occured.
     *  \pre g is undirected, simple and loopfree.
     *  \pre len is non-negative
     *  \remark Care must be taken when the template parameter is instantiated with a data type
     *          which has rounding errors.
     *  \ingroup exactmcb
     */
    template<class W>
    W UMCB_FH( const graph& g, 
                const edge_array<W>& len,
                array< mcb::spvecgf2 >& mcb, 
                const mcb::edge_num& enumb ) 
    { 
        array< mcb::spvecgf2 > proof;
        WeightedSPTreesSupportMCB<W,mcb::spvecgf2> tmp( g, len, mcb, proof, enumb );
        return tmp.run();
    }

    //@}

} // namespace mcb end

#endif  // UMCB_H

/* ex: set ts=4 sw=4 sts=4 et: */


