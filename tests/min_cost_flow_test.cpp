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

static void EmptyGraphTest(httplib::Client* cli);
static void SimpleTest(httplib::Client* cli);
static void RandomTest(httplib::Client* cli);
static void RandomTestHelper(httplib::Client* cli, const std::string& graphType);

void TestMinCostFlow(httplib::Client* cli) {
    TestSuite suite("TestMinCostFlow");

    RUN_TEST_REMOTE(suite, cli, EmptyGraphTest);
    RUN_TEST_REMOTE(suite, cli, SimpleTest);
    RUN_TEST_REMOTE(suite, cli, RandomTest);
}
/**
 * @brief Простейший статический тест для пустого графа.
 *
 * @param cli Указатель на HTTP клиент.
 */
 static void EmptyGraphTest(httplib::Client* cli) {

    nlohmann::json input = R"(
  {
    "graph_type": "Graph",
    "vertices": [ ],
    "edges": [ ]
  }
  )"_json;

    auto res = cli->Post("/MinCostFlow", input.dump(), "application/json");
  
    if (!res) {
      REQUIRE(false);
    }

    nlohmann::json output = nlohmann::json::parse(res->body);
  
    std::vector<int> result = output.at("result");
  
    std::unordered_set<int> expected;
    std::unordered_set<int> resultSet;
  
    for (int id : result)
      resultSet.insert(id);
  
    REQUIRE_EQUAL(expected, resultSet);
}
/**
 * @brief Простейший статический тест.
 *
 * @param cli Указатель на HTTP клиент.
 */
static void SimpleTest(httplib::Client* cli) {
  nlohmann::json input;

  input["graph_type"] = "Graph";
  input["vertices"] = std::vector<int>{1, 3, 5, 7, 9, 11};
  input["edges"][0]["from"] = 1;
  input["edges"][0]["to"] = 3;

  input["edges"][1]["from"] = 3;
  input["edges"][1]["to"] = 7;

  input["edges"][2]["from"] = 1;
  input["edges"][2]["to"] = 9;

  input["edges"][3]["from"] = 5;
  input["edges"][3]["to"] = 11;

  auto res = cli->Post("/MinCostFlow", input.dump(), "application/json");

  if (!res) {
    REQUIRE(false);
  }

  nlohmann::json output = nlohmann::json::parse(res->body);

  std::vector<int> result = output.at("result");

  std::unordered_set<int> expected = {1, 3, 5, 7, 9, 11};
  std::unordered_set<int> resultSet;

  for (int id : result)
    resultSet.insert(id);

  REQUIRE_EQUAL(expected, resultSet);
}

/**
 * @brief Простейший случайный тест.
 *
 * @param cli Указатель на HTTP клиент.
 */
static void RandomTest(httplib::Client* cli) {
  RandomTestHelper(cli, "Graph");
  RandomTestHelper(cli, "OrientedGraph");
}

/**
 * @brief Простейший случайный тест.
 *
 * @param cli Указатель на HTTP клиент.
 * @param graphType Тип графа.
 */
static void RandomTestHelper(httplib::Client* cli, const std::string& graphType) {
  const int numTries = 100;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<size_t> vertexId(100, 150);
  std::uniform_int_distribution<size_t> verticesSize(10, 100);
  std::uniform_int_distribution<size_t> edgesSize(10, 300);

  for (int it = 0; it < numTries; it++) {
    size_t numEdges = edgesSize(gen);

    std::vector<size_t> vertices;

    nlohmann::json input;

    input["graph_type"] = graphType;

    size_t numRealEdges = 0;
    for (size_t i = 0; i < numEdges; i++) {
      size_t id1 = vertexId(gen);
      size_t id2 = vertexId(gen);

      if (id1 == id2)
        continue;

      vertices.push_back(id1);
      vertices.push_back(id2);

      input["edges"][numRealEdges]["from"] = id1;
      input["edges"][numRealEdges]["to"] = id2;
      numRealEdges++;
    }

    input["vertices"] = vertices;

    auto res = cli->Post("/MinCostFlow", input.dump(), "application/json");

    if (!res) {
      REQUIRE(false);
    }

    /* Используем метод parse() для преобразования строки ответа сервера
    (res->body) в объект JSON. */
    nlohmann::json output = nlohmann::json::parse(res->body);

    /* Проверка результатов сортировки. */
    std::vector<size_t> result = output.at("result");

    /* При генерации случайных вершин могли получиться одинаковые.
     * Сортируем массив и удаляем повторяющиеся элементы. */
    std::sort(vertices.begin(), vertices.end());
    vertices.erase(std::unique(vertices.begin(), vertices.end()),
        vertices.end());

    /* Сортируем результат обхода графа. */
    std::sort(result.begin(), result.end());

    REQUIRE_EQUAL(vertices, result);
  }
}