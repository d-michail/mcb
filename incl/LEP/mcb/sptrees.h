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

/*! \file sptrees.h
 *  \brief Implementation of the Fast Hybrid MCB algorithm. The algorithm 
 *         uses the collection of cycles defined by Horton. The cycles are 
 *         represented as \f$n\f$ shortest path trees, one for each vertex. 
 *         Then, using the normal support vectors (witnesses), it extracts
 *         the cycles from this collection.
 *
 *  \author Dimitrios Michail
 */

#ifndef SPTREES_H
#define SPTREES_H

#include <LEP/mcb/config.h>
#include <LEP/mcb/spvecgf2.h>
#include <LEP/mcb/count_ptr.h>
#include <LEP/mcb/udijkstra.h>

#ifdef LEDA_GE_V5
#include <LEDA/graph/graph.h>
#include <LEDA/graph/edge_array.h>
#include <LEDA/graph/node_array.h>
#include <LEDA/core/map.h>
#include <LEDA/core/array.h>
#include <LEDA/core/list.h>
#include <LEDA/core/d_int_set.h>
#else
#include <LEDA/graph.h>
#include <LEDA/edge_array.h>
#include <LEDA/node_array.h>
#include <LEDA/map.h>
#include <LEDA/array.h>
#include <LEDA/list.h>
#include <LEDA/d_int_set.h>
#endif

#include <vector>
#include <algorithm>

// start our namespace
namespace mcb 
{ 

#if defined(LEDA_NAMESPACE)
    using leda::graph;
    using leda::array;
    using leda::edge;
    using leda::edge_array;
    using leda::node;
    using leda::node_array;
    using leda::list;
    using leda::error_handler;
    using leda::d_int_set;
#endif

    template<class W, class T> class TreeNode;
    template<class W, class T> class ShortestPathTree;

    template<class W, class T>
        class TreeNode {
            public:
                TreeNode() : length_( W() ), info_( T() ) {}

                ~TreeNode() {}

                void add_child( TreeNode<W,T>* c ) { 
                    children_.push_back( c ); 
                }

                list< TreeNode<W,T>* >& children() { 
                    return children_; 
                }

                T& info() { return info_; }

                W& length() { return length_; }

            private: 
                W length_;
                T info_;
                list< TreeNode<W, T>* > children_;
        };

    template<class W, class T>
        class ShortestPathTree 
        {
            public: 
                ShortestPathTree( const graph& g, 
                         const node s, 
                         const edge_array<W>& length,
                         const node_array<edge>& pred 
                       )
                    : G(g), S(s), len(length), root(nil), map(G,nil), pr(nil) 
                {
                    initialize( pred );
                }

                ~ShortestPathTree() 
                {
                    node v; 
                    forall_nodes( v, G ) 
                        if ( map[v] != nil ) 
                            delete map[v];
                }

                bool empty() const { 
                    return root == nil; 
                }

                /*! \brief Get the shortest path tree node of a graph node. 
                 */
                TreeNode<W,T>* get_TreeNode( node v ) const
                {
                    return map[ v ];
                }

                // DEBUG DEBUG
                node get_node( TreeNode<W,T>* v ) const { return map_debug[ v ]; }

                TreeNode<W,T>* getRoot() const { 
                    return root; 
                }

                /*! Get the predecessor edge of a graph node 
                 *  in the shortest path tree 
                 */
                edge get_pred( TreeNode<W,T>* v ) const {
                    if ( pr.defined( v ) == false ) 
                        return nil;
                    return pr[v]; 
                }

                /*! Compute the length of the cycle consisting
                 *  of an edge and the two shortest paths from the 
                 *  root of the shortest path tree to the endpoints of
                 *  the edge.
                 */
                W ComputeCycleLength( const edge e ) {
                    W length = len[e];
                    TreeNode<W,T>* t = get_TreeNode( G.source(e) );
                    if ( t != nil )
                        length += t->length();
                    TreeNode<W,T>* s = get_TreeNode( G.target(e) );
                    if ( s != nil ) 
                        length += s->length();
                    return length;
                }

                
            private:
                ShortestPathTree( const ShortestPathTree<W,T>& ); 
                ShortestPathTree<W,T>& operator=( const ShortestPathTree<W,T>& );

                void CreateNodeMap( const node_array<edge>& pred )
                {
                    node v;

                    forall_nodes( v, G ) {
                        if ( v == S || pred[v] != nil ) { // was visited
                            map[ v ] = new TreeNode<W,T>();
                            map_debug[ map[v] ] = v; // DEBUG DEBUG
                            pr[ map[v] ] = pred[ v ];
                        }
                    }    
                }

                void LinkTree( const node_array<edge>& pred )
                {
                    node v;
    
                    forall_nodes( v, G ) { 
                        if ( pred[ v ] != nil ) { // was visited 
                            map[ G.opposite( pred[v], v ) ]->add_child( map[ v ] );
                        }
                        else if ( v == S ) { 
                            root = map[v]; 
                        } 
                    }
                }

                /*! Compute shortest path tree lengths.  
                 *  \param len The edge lengths.
                 */
                void ComputeLengths()
                {
                    // fix root's info
                    TreeNode<W,T>* r = getRoot(); 
                    if ( r == nil ) return;
                    r->length() = W();

                    // init list, add root
                    list< TreeNode< W,T >* > l;
                    l.push_back( r );

                    // traverse and update lengths
                    TreeNode< W,T > *p, *c;
                    edge e = nil;
                    while( l.empty() == false ) 
                    {
                        p = l.pop_front();

                        leda::list_item li = p->children().first();
                        while( li != nil ) {
                            c = p->children().contents( li );
                            e = get_pred( c );
                            c->length() = p->length() + len[e];
                            l.push_back(c);
                            li = p->children().succ( li );
                        }
                    }
                }


                void initialize( const node_array<edge>& pred ) 
                { 
                    CreateNodeMap( pred );
                    LinkTree( pred );
                    ComputeLengths();
                }

                // private data
                const graph& G;
                const node S;
                const edge_array<W>& len;

                TreeNode<W,T> *root;
                node_array< TreeNode<W,T>* > map;
                leda::map< TreeNode<W,T>*, edge > pr;
                leda::map< TreeNode<W,T>*, node > map_debug;  // DEBUG DEBUG
        };


    template<class W>
        class CandidateCycle
        { 
            public:
                CandidateCycle( node i, edge j ) : v(i), e(j), len(W()) {}
                CandidateCycle( node i, edge j, W l ) : v(i), e(j), len(l) {}

                node getNode() const { return v; }
                edge getEdge() const { return e; }
                W&   getLength() const { return len; }

                bool operator<( const CandidateCycle<W>& c ) const { return len < c.len; }

            private:
                node v; 
                edge e;
                W len;
        };


    template<class W, class T>
    class HortonSupersetTrees
    {
        public:
            HortonSupersetTrees( const graph& g_, 
                                 const edge_array<W>& len_ )
                : g(g_), len(len_)
            {
                computeTreesAndCycles();
            }

            virtual ~HortonSupersetTrees() {}

        private:
            HortonSupersetTrees( const HortonSupersetTrees& );
            HortonSupersetTrees& operator=( const HortonSupersetTrees& );
            

            void computeTreesAndCycles() { 
                node v,u; 
                edge e;
                node_array<W> dist( g );
                node_array<edge> pred( g );
                trees.init( g );
                edge_array<bool> used( g, false );

                forall_nodes( v, g ) {

                    pred.init( g, nil );
                    mcb::undirected_dijkstra( g, v, len, dist, pred );

                    //std::cout << "making tree for vertex " << vname[ v ] << std::endl;
                    trees[ v ] = count_ptr< ShortestPathTree<W,T> >( new ShortestPathTree<W,T>( g, v, len, pred ) );

                    forall_nodes( u , g )
                        if ( pred[u] != nil )
                            used[ pred[u] ] = true;

                    forall_edges( e, g ) 
                        if ( used[ e ] == false && 
                             trees[v]->get_TreeNode( g.source(e) ) != nil &&
                             trees[v]->get_TreeNode( g.target(e) ) != nil ) 
                        { 
                            cycles.push_back( CandidateCycle<W>( v, e, trees[v]->ComputeCycleLength( e ) ) );
                        }

                    forall_nodes( u , g ) 
                        if ( pred[u] != nil ) 
                            used[ pred[u] ] = false;
                }
                std::sort( cycles.begin(), cycles.end() );
            }

        protected:
            const graph& g;
            const edge_array<W>& len;
            node_array< count_ptr< ShortestPathTree<W,T> > > trees;
            std::vector< CandidateCycle<W> > cycles;
    };

    /************************** UNDIRECTED GRAPHS ***************************/

    template<class W>
    class UndirectedHortonSupersetTrees: public HortonSupersetTrees<W,bool>
    {
        public: 
            typedef HortonSupersetTrees<W,bool> base_type;

            UndirectedHortonSupersetTrees( const graph& g_, 
                                           const edge_array<W>& len_, 
                                           const edge_num& enumb_ )
                : base_type( g_, len_ ), enumb(enumb_) 
            { 
            }

            ~UndirectedHortonSupersetTrees() {}

            void updateTreeLabels( const d_int_set& support ) 
            { 
                node v;
                forall_nodes( v, g ) 
                    updateTreeLabels( v, support );
            }

            void updateTreeLabels( const spvecgf2& support ) 
            { 
                updateTreeLabels( support.to_d_int_set() );
            }

            W getShortestOddCycle( const spvecgf2& S, spvecgf2& C ) 
            {
                return getShortestOddCycle( S.to_d_int_set(), C ); 
            }

        private:
            UndirectedHortonSupersetTrees( const UndirectedHortonSupersetTrees& );
            UndirectedHortonSupersetTrees& operator=( const UndirectedHortonSupersetTrees& );

            W getShortestOddCycle( const d_int_set& support, spvecgf2& C ) 
            {
                node v; 
                edge e;
                bool label;
                W min = W();
                int min_index = -1;

                TreeNode<W,bool> *eu, *ew;
                for( unsigned int i = 0; i < cycles.size(); i++ ) 
                {
                    v = cycles[i].getNode();
                    e = cycles[i].getEdge();

                    eu = trees[v]->get_TreeNode( g.source(e) );
                    ew = trees[v]->get_TreeNode( g.target(e) );

#if ! defined(LEDA_CHECKING_OFF)
                    if ( eu == nil || ew == nil ) 
                        continue;
#endif
                    label = eu->info() ^ ew->info() ^ support.member( enumb(e) );

                    if ( label ) { 
                        min = eu->length() + ew->length() + len[e];
                        min_index = i;
                        break;
                    }
                }

                if ( min_index < 0 ) 
                    error_handler(999,"HF_MCB: no cycle found, this might be a bug!");

                constructCycleUndirected( cycles[min_index], C); 

                return min;
            }


            void constructCycleUndirected( const CandidateCycle<W>& cc, spvecgf2& C ) 
            {
                spvecgf2 C1;
                constructPathUndirected( cc.getNode(), g.source( cc.getEdge() ), C1 ); 

                spvecgf2 C2;
                constructPathUndirected( cc.getNode(), g.target( cc.getEdge() ), C2 );

                C.clear();
                C.insert( enumb( cc.getEdge() ) );

                // NOTE: If the graph has edges with zero length, paths C1,C2 might 
                //       contain common edges. The following addition will eliminate them.
                C += C1;
                C += C2;
            }

            // construct path from v to root in trees[root]
            void constructPathUndirected( node root, node v, spvecgf2& C ) 
            {
                edge e; 
                node u = v; 
                e = trees[root]->get_pred( trees[root]->get_TreeNode( u ) );
                while( e != nil ) {
                    C.insert( enumb( e ) );
                    u = g.opposite( u, e );
                    e = trees[root]->get_pred( trees[root]->get_TreeNode( u ) );
                }
                C.sort();
            }

            void updateTreeLabels( node v, const d_int_set& support ) { 
                // fix root's info
                TreeNode< W, bool >* r = trees[v]->getRoot(); 
                if ( r == nil ) return;
                r->info() = false;

                // init list, add root
                list< TreeNode< W, bool >* > l;
                l.push_back( r );

                // traverse and update labels
                TreeNode< W, bool > *p, *c;
                edge e;
                while( l.empty() == false ) 
                {
                    p = l.pop_front();

                    leda::list_item li = p->children().first();
                    while( li != nil ) {
                        c = p->children().contents( li );
                        e = trees[v]->get_pred( c );
                        c->info() = p->info() ^ support.member( enumb(e) );
                        l.push_back(c);
                        li = p->children().succ( li );
                    }
                }
            }

            const edge_num& enumb;
            using base_type::g;
            using base_type::trees;
            using base_type::cycles;
            using base_type::len;
    };

}

#endif  // SPTREES_H

/* ex: set ts=4 sw=4 sts=4 et: */
