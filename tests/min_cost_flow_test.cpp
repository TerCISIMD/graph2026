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

#include <iostream>
static void FirstTest(httplib::Client* cli);

void TestMinCostFlow(httplib::Client* cli) {
  TestSuite suite("TestMinCostFlow");

  RUN_TEST_REMOTE(suite, cli, FirstTest);
}

static void FirstTest(httplib::Client* cli){
  nlohmann::json input;

  input["graph_type"] = "WeightedOrientedGraph";
  input["vertices"] = std::vector<int>{0, 1, 2, 3};
  input["weight_type"] = "<int, int>";

  input["edges"][0]["from"] = 0;
  input["edges"][0]["to"] = 1;
  input["edges"][0]["weights_1"] = 3;
  input["edges"][0]["weights_2"] = 1;

  input["edges"][1]["from"] = 0;
  input["edges"][1]["to"] = 2;
  input["edges"][1]["weights_1"] = 3;
  input["edges"][1]["weights_2"] = 5;

  input["edges"][2]["from"] = 1;
  input["edges"][2]["to"] = 2;
  input["edges"][2]["weights_1"] = 2;
  input["edges"][2]["weights_2"] = 2;

  input["edges"][3]["from"] = 1;
  input["edges"][3]["to"] = 3;
  input["edges"][3]["weights_1"] = 3;
  input["edges"][3]["weights_2"] = 8;

  input["edges"][4]["from"] = 2;
  input["edges"][4]["to"] = 3;
  input["edges"][4]["weights_1"] = 3;
  input["edges"][4]["weights_2"] = 3;

  input["flow_cost"] = 4;
  input["begin_at"] = 0;
  input["end_at"] = 3;

  auto res = cli->Post("/MinCostFlow", input.dump(), "application/json");

  if (!res) {
    REQUIRE(false);
  }

  nlohmann::json output = nlohmann::json::parse(res->body);
  std::vector<int> result = output.at("result");

  std::unordered_set<int> expected = { 0, 3 };
  std::unordered_set<int> resultSet;

  for (int id : result)
    resultSet.insert(id);

  REQUIRE_EQUAL(expected, resultSet);
}