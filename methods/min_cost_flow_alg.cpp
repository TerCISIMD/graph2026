#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <graph.hpp>
#include <oriented_graph.hpp>
#include <weighted_graph.hpp>
#include <weighted_oriented_graph.hpp>
#include <min_cost_flow.hpp>
// #include <iostream>

using graph::Graph;
using graph::OrientedGraph;
using graph::WeightedGraph;
using graph::WeightedOrientedGraph;

namespace graph {

int MinCostFlowAuth(const nlohmann::json& input, nlohmann::json* output);

int MinCostFlowAlg(const nlohmann::json& input, nlohmann::json* output) {
    std::string graphType = input.at("graph_type");

    /*
    У каждого ребра должна быть пропускная способность и стоимость потока, поэтому изначальный граф должен быть взвешенный.
    */

    if (graphType == "Graph") {
        return -1;
    } else if (graphType == "OrientedGraph") {
        return -1;
    } else if (graphType == "WeightedGraph") {
        std::string weightType = input.at("weight_type");
        if (weightType == "<int, int>") {
            return MinCostFlowAuth(input, output);
        } else {
            return -1;
        }
    } else if (graphType == "WeightedOrientedGraph") {
        std::string weightType = input.at("weight_type");
        if (weightType == "<int, int>") {
            return MinCostFlowAuth(input, output);
        } else {
            return -1;
        }
    }

    return -1;
}

/**
 * @brief Метод увеличивающих путей.
 *
 * @param input Входные данные в формате JSON.
 * @param output Выходные данные в формате JSON.
 * @return Функция возвращает 1 в случае успеха и отрицательное число
 * если входные данные заданы некорректно.
 *
 * Функция запускает алгоритм увеличивающих путей, используя входные данные
 * в JSON формате. Результат также выдаётся в JSON формате.
 */

int MinCostFlowAuth(const nlohmann::json& input, nlohmann::json* output) {
    WeightedGraph<rib> graph;
    int flow_cost = input.at("flow_cost");
    auto g = [&graph](size_t v) { std::vector<rib> ribs;
        for (auto& vert : graph.Edges(v)) { auto& r = graph.EdgeWeight(v, vert); 
          ribs.push_back({r.b, r.u, r.c, r.f, r.back}); }
        return ribs;
    };
    int s = input.at("begin_at");
    int t = input.at("end_at");

    for (auto& vertex : input.at("vertices")) {
      graph.AddVertex(vertex);
    }
    // std::cout << 1 << std::endl;
    for (auto& edge : input.at("edges")) {
        graph.AddEdge(edge.at("from"), edge.at("to"), { edge.at("to"), 
            static_cast<int>(edge.at("weights_1")),
            edge.at("weights_2"), 0,  g(edge.at("to")).size() });
        graph.AddEdge(edge.at("to"), edge.at("from"), { edge.at("from"), 0,
            -static_cast<int>(edge.at("weights_2")), 0,
            g(edge.at("from")).size() });
    }
    // std::cout << 2 << std::endl;
    MinCostFlow(graph, flow_cost, s, t, output);
    // std::cout << 3 << std::endl;
    return 1;
}
}  // namespace graph
