/**
 * @file min_cost_flow_test.cpp
 * @author Pavlov Fedor
 *
 * Тесты для алгоритма graph::MinCostFlow.
 */

#include <httplib.h>
#include <unordered_set>
#include <string>
#include <vector>
#include <algorithm>
#include <nlohmann/json.hpp>
#include "test_core.hpp"

void TestMinCostFlow(httplib::Client* cli) {
  nlohmann::json input;

  input["graph_type"] = "WeightedOrientedGraph";
  input["vertices"] = std::vector<int>{1, 2, 3, 4};
  input["weightType"] = "pair<int, int>";

  input["edges"][0]["from"] = 1;
  input["edges"][0]["to"] = 2;
  input["edges"][0]["weights"]["0"] = 3;
  input["edges"][0]["weights"]["1"] = 1;

  input["edges"][1]["from"] = 1;
  input["edges"][1]["to"] = 3;
  input["edges"][1]["weights"]["0"] = 3;
  input["edges"][1]["weights"]["1"] = 5;

  input["edges"][2]["from"] = 2;
  input["edges"][2]["to"] = 3;
  input["edges"][2]["weights"]["0"] = 2;
  input["edges"][2]["weights"]["1"] = 2;

  input["edges"][3]["from"] = 2;
  input["edges"][3]["to"] = 4;
  input["edges"][3]["weights"]["0"] = 3;
  input["edges"][3]["weights"]["1"] = 8;

  input["edges"][4]["from"] = 3;
  input["edges"][4]["to"] = 4;
  input["edges"][0]["weights"]["0"] = 3;
  input["edges"][0]["weights"]["1"] = 3;

  input["flow_cost"] = 4;
  input["begin at"] = 1;
  input["end at"] = 4;

  auto res = cli->Post("/MinCostFlow", input.dump(), "application/json");

  if (!res) {
    REQUIRE(false);
  }

  nlohmann::json output = nlohmann::json::parse(res->body);

  int cost = output.at("result");
  int expected = 29;

  REQUIRE_EQUAL(expected, cost);
}