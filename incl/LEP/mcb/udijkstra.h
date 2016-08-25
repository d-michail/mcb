
#ifndef UDIJKSTRA_H
#define UDIJKSTRA_H

#include <LEP/mcb/config.h>

#ifdef LEDA_GE_V5
#include <LEDA/graph/graph.h>
#include <LEDA/graph/node_pq22.h>
#else
#include <LEDA/graph.h>
#include <LEDA/node_pq22.h>
#endif
#include <limits>

namespace mcb 
{

#if defined(LEDA_NAMESPACE)
	using leda::graph;
	using leda::node;
	using leda::edge;
	using leda::edge_array;
	using leda::node_array;
#endif

	template <class W>
		void undirected_dijkstra(const graph& G, node s, 
				const edge_array<W>& len, 
				node_array<W>& dist,
				node_array<leda::edge>& pred)
		{
			int n = G.number_of_nodes();
			int m = G.number_of_edges();

			leda::node_pq22<W,graph> PQ(n+m); 

#if defined(LEDA_STD_HEADERS) || defined(__mipspro__) || defined(_MSC_VER) || defined(__DMC__)
			W max_dist = std::numeric_limits<W>::max();
#else
			W max_dist = numeric_limits<W>::max();
#endif

			node v;
			forall_nodes(v,G) 
			{ dist[v] = max_dist;
				pred[v] = 0;
			}

			dist[s] = 0;
			PQ.insert(s,0);

			while (!PQ.empty())
			{ W du;
				node u = PQ.del_min(du,dist);  

				if (du != dist[u]) continue;

				edge e;
				forall_adj_edges(e,u)
				{ node v = G.opposite(e,u);
					W c = du + len[e]; 
					if (c < dist[v]) 
					{ PQ.insert(v,c); 
						dist[v] = c; 
						pred[v] = e;                           
					}
				}                                                                  
			}
		}  

} // namespace mcb

#endif

/* ex: set ts=4 sw=4 sts=4 et: */
