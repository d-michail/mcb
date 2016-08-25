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

/*! \file ushortpath.h
 *  \brief Undirected shortest paths using Dijkstra's algorithm.
 *
 */
#ifndef USHORTPATH_H
#define USHORTPATH_H

#include <LEP/mcb/config.h>

#ifdef LEDA_GE_V5
#include <LEDA/graph/graph.h>
#include <LEDA/graph/edge_array.h>
#include <LEDA/graph/node_array.h>
#include <LEDA/core/array.h>
#include <LEDA/core/list.h>
#include <LEDA/core/impl/f_heap.h>
#include <LEDA/core/impl/bin_heap.h>
#include <LEDA/graph/node_pq.h>
#else
#include <LEDA/graph.h>
#include <LEDA/edge_array.h>
#include <LEDA/node_array.h>
#include <LEDA/array.h>
#include <LEDA/list.h>
#include <LEDA/impl/f_heap.h>
#include <LEDA/impl/bin_heap.h>
#include <LEDA/node_pq.h>
#endif

// start our namespace
namespace mcb 
{ 

#if defined(LEDA_NAMESPACE)
    using leda::graph;
    using leda::array;
    using leda::node;
    using leda::node_array;
    using leda::edge;
    using leda::edge_array;
    using leda::list;
    using leda::node_pq;
    using leda::error_handler;
#endif

    namespace detail 
    {

	/*! A class to provide undirected weighted single source single target 
	 *  shortest path queries. This is especially useful if shortest 
	 *  paths between several pair of nodes are requested.
	 *
	 *  \author Dimitrios Michail
	 *  \date 2006
	 */
	template<class W, class prio_impl = leda::bin_heap>
	    class ushortestpaths
	    {
		public:
		    ushortestpaths( const graph& G, const edge_array<W>& l ) 
			: g(G), 
			len(l), 
			_pred(G, nil), 
			_dist( G, W() ), 
			// bin heap fails if called with empty graph
			pq( G, G.number_of_nodes() > 0 ? G.number_of_nodes() : 1 ),
			last_s( nil )
			{}

		    ~ushortestpaths() {}

		    /*! Compute a shortest path from a source node to a target node.
		     *  \param s The source node
		     *  \param t The target node
		     */
		    void compute_shortest_path( const node& s, const node& t ) 
		    {
			// init pred array from previous runs
			while( touched.empty() == false ) {
			    _pred[ touched.pop() ] = nil;
			}
			pq.clear();

			node u,w;
			edge e;

			// set initial values
			_pred[s] = nil;
			_dist[s] = 0;
			last_s = s;
			touched.push(s);
			pq.insert( s, 0 );

			// main loop
			while( ! pq.empty() ) 
			{ 
			    u = pq.del_min(); // del min

			    if ( u == t )   // found target
				return;

			    // update or add
			    if ( g.is_undirected() )
				// code duplication due to LEDA-6.0 change in macro
				// forall_inout_edges
				// Prior to 6.0 only one loop with forall_inout_edges worked
				// for both undirected and directed graphs
				forall_adj_edges( e, u ) 
				{
				    w = g.opposite( u, e );
				    W c = _dist[u] + len[ e ];

#if !defined(LEDA_CHECKING_OFF)
				    if ( c < 0 )
					error_handler(999,"USHORTPATH: negative distance (overflow?)");
#endif
				    if ( _pred[w] == nil && w != s ) { // add
					pq.insert( w, c );
					touched.push( w );
				    }
				    else if ( c < _dist[w] ) { // decrease key
					pq.decrease_p( w, c );
				    }
				    else continue;

				    _dist[w] = c;
				    _pred[w] = e;
				}
			    else
				forall_inout_edges( e, u ) 
				{
				    w = g.opposite( u, e );
				    W c = _dist[u] + len[ e ];

#if !defined(LEDA_CHECKING_OFF)
				    if ( c < 0 )
					error_handler(999,"USHORTPATH: negative distance (overflow?)");
#endif
				    if ( _pred[w] == nil && w != s ) { // add
					pq.insert( w, c );
					touched.push( w );
				    }
				    else if ( c < _dist[w] ) { // decrease key
					pq.decrease_p( w, c );
				    }
				    else continue;

				    _dist[w] = c;
				    _pred[w] = e;
				}
			}
		    }

		    /*! Get the predecessor edge of a node in the last shortest path computation.
		     *  \param v A node
		     *  \return The predecessor edge of a node
		     */
		    edge pred( const node& v ) { return _pred[v]; }

		    /*! Get the distance of a node from the source node in the last 
		     *  shortest path computation.
		     *  \param v A node v
		     *  \return The distance of v from the source node of the last shortest path
		     *          computation. If the node was not reachable this value is undefined.
		     */
		    W dist( const node& v ) { 
#if ! defined(LEDA_CHECKING_OFF)
			if ( _pred[v] == nil ) 
			    std::cerr << "ushortestpaths: returning distance of unreachable vertex!" << std::endl;
#endif
			return _dist[v]; 
		    }

		    /*! Check if a node v is reachable from the source node of the last shortest path
		     *  computation.
		     *  \param v A node v
		     *  \return True if v is reachable from the source node of the last shortest path 
		     *	    computation
		     */
		    bool is_reachable( const node& v ) { 
			if ( v == last_s )
			    return true;
			return _pred[v] != nil; 
		    }

		private:
		    const graph& g;
		    const edge_array<W>& len;

		    list<node> touched;
		    node_array<edge> _pred;
		    node_array<W> _dist;
		    node_pq<W,graph,prio_impl> pq;
		    node last_s;
	    };


	/*! A class to provide undirected and unweighted single source single target 
	 *  shortest path queries. This is especially useful if shortest 
	 *  paths between several pair of nodes are requested.
	 *
	 *  \author Dimitrios Michail
	 *  \date 2006
	 */
	class ubfs
	{
	    public:
		/*! Constructor
		 *  \param G The graph to do BFS in
		 */
		ubfs( const graph& G );

		/*! Destructor
		*/
		~ubfs();

		/*! Compute a shortest path from a source node to a target node.
		 *  \param s The source node
		 *  \param t The target node
		 *  \param limit Reach nodes up to this depth.
		 */
		void compute_shortest_path( const node& s, const node& t, int limit );

		/*! Get the predecessor edge of a node in the last shortest path computation.
		 *  \param v A node
		 *  \return The predecessor edge of a node
		 */
		edge pred( const node& v ) { return pred_[v]; }

		/*! Get the distance of a node from the source node in the last 
		 *  shortest path computation.
		 *  \param v A node v
		 *  \return The distance of v from the source node of the last shortest path
		 *          computation. If the node was not reachable this value is undefined.
		 */
		int dist( const node& v ) { 
#if ! defined(LEDA_CHECKING_OFF)
		    if ( pred_[v] == nil ) 
			std::cerr << "ushortestpaths: returning distance of unreachable vertex!" << std::endl;
#endif
		    return dist_[v]; 
		}

		/*! Check if a node v is reachable from the source node of the last shortest path
		 *  computation.
		 *  \param v A node v
		 *  \return True if v is reachable from the source node of the last shortest path 
		 *	    computation
		 */
		bool is_reachable( const node& v ) { 
		    if ( v == last_s )
			return true;
		    return pred_[v] != nil; 
		}

	    private:
		const graph& g;

		list<node> Q;
		node_array<edge> pred_;
		node_array<int> dist_;
		node last_s;
	};

    }

}  // end of namespace mcb

#endif  // UMCB_APPROX_H

/* ex: set ts=8 sw=4 sts=4 noet: */


