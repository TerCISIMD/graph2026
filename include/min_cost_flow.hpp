/**
 * @file include/min_cost_flow.hpp
 * @author Pavlov Fedor
 *
 * Алгоритм увеличивающих путей.
 */

#ifndef INCLUDE_MIN_COST_FLOW_HPP_
#define INCLUDE_MIN_COST_FLOW_HPP_
#include <vector>
#include <map>

// #include <iostream>
struct rib {
  int b, u, c, f;
  size_t back;
};

namespace graph {

/**
 * @brief алгоритм увеличивающих путей
 *
 * @param graph Граф для оалгоритма.
 * @param cost_flow Стоимость потока.
 * @param s исток
 * @param t сток
 * @param output результат
 */
void MinCostFlow(WeightedGraph<rib> graph, int cost_flow, int s, int t,
    nlohmann::json* output) {
    const int INF = 1000*1000*1000;
    size_t n = graph.NumVertices();
    auto g = [&graph](size_t v) { std::vector<rib*> ribs;
      for (auto& vert : graph.Edges(v))
      ribs.push_back(&graph.EdgeWeight(v , vert));;
      return ribs;
    };
    int flow = 0,  cost = 0;
	std::vector<int> last_path;

    while (flow < cost_flow) {

		// std::cout << "flow = " << flow << std::endl;
	  std::vector<int> current_path;
	  std::map<int, int> id;
	  std::map<int, int> d;
	  std::map<int, int> q;
	  std::map<int, int> p;
	  std::map<int, size_t> p_rib;
	  size_t qh = 0, qt = 0;

	  for (int i = 0; i < n; i++) {
		  d[i] = INF;
		  id[i] = 0;
	  }
	  q[qt++] = s;
	  d[s] = 0;
	  while (qh != qt) {
		  int v = q[qh++];
		  id[v] = 2;
		  if (qh == n)  qh = 0;
		  for (size_t i = 0; i < g(v).size(); ++i) {
			  rib* r = g(v)[i];
			  if (r->f < r->u && d[v] + r->c < d[r->b]) {
				  d[r->b] = d[v] + r->c;
				  if (id[r->b] == 0) {
					  q[qt++] = r->b;
					  if (qt == n)  qt = 0;
				  }
				  else if (id[r->b] == 2) {
					  if (--qh == -1)  qh = n - 1;
					  q[qh] = r->b;
				  }
				  id[r->b] = 1;
				  p[r->b] = v;
				  p_rib[r->b] = i;
			  }
		  }
	  }
		
      for (int v = t; v != s; v = p[v]) {
          current_path.push_back(v);
      }
      current_path.push_back(s);
      std::reverse(current_path.begin(), current_path.end());
	  last_path = current_path;

		// for(int i = 0; i < last_path.size(); i++){
			// std::cout << last_path[i] << std::endl;
		// }

	  if (d[t] == INF)  break;

	  int addflow = cost_flow - flow;
	  for (int v = t; v != s; v = p[v]) {
		int pv = p[v];  size_t pr = p_rib[v];
		addflow = std::min (addflow, g(pv)[pr]->u - g(pv)[pr]->f);
	  }
	  for (int v = t; v != s; v = p[v]) {
		int pv = p[v];  size_t pr = p_rib[v],  r = g(pv)[pr]->back;
		g(pv)[pr]->f += addflow;
		g(v)[r]->f -= addflow;
		cost += g(pv)[pr]->c * addflow;
	  }
	  flow += addflow;

	  for (int v = t; v != s; v = p[v]) {
        current_path.push_back(v);
      }
      current_path.push_back(s);
      std::reverse(current_path.begin(), current_path.end());
	  last_path = current_path;

	  // for(int i = 0; i < last_path.size(); i++){
		// std::cout << last_path[i] << std::endl;
	  // }

      }
	(*output)["result"] = last_path; 
}

} // namespace graph
#endif  // INCLUDE_MIN_COST_FLOW_HPP_
