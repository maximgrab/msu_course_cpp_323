#include <filesystem>
#include <string>
#include "config.hpp"
#include "fstream"
#include "graph.hpp"
#include "graph_generation_controller.hpp"
#include "graph_generator.hpp"
#include "graph_json_printing.hpp"
#include "graph_printing.hpp"
#include "iostream"
#include "logger.hpp"
using std::string;
void prepareTempDirectory() {
  std::filesystem::create_directory(uni_course_cpp::config::kTempDirectoryPath);
}
void writeToFile(string const& output, string const& filename) {
  std::ofstream fileToWrite(uni_course_cpp::config::kTempDirectoryPath +
                            filename);
  fileToWrite << output;
}
string generationStartedString(int i) {
  return ("Graph " + std::to_string(i) + ", generation started");
}
string generationFinishedString(int i, std::string string) {
  return ("Graph " + std::to_string(i) + ", generation finished" + string);
}

int intInput(string inputMessage) {
  int input = -1;
  std::cout << inputMessage;
  std::cin >> input;
  while ((std::cin.fail() || input < 0)) {
    std::cout << ((input < 0) ? ("Error: input cannot be <0")
                              : "Error: input type != int")
              << "\n";
    std::cin.clear();
    std::cin.ignore(256, '\n');
    std::cout << "Input " << inputMessage;
    std::cin >> input;
  }
  return input;
};

std::vector<uni_course_cpp::Graph> generateGraphs(
    const uni_course_cpp::GraphGenerator::Params& params,
    int graphs_count,
    int threads_count) {
  auto generation_controller = uni_course_cpp::GraphGenerationController(
      threads_count, graphs_count, params);

  auto& logger = uni_course_cpp::Logger::getLogger();

  auto graphs = std::vector<uni_course_cpp::Graph>();
  graphs.reserve(graphs_count);

  generation_controller.generate(
      [&logger](int i) { logger.log(generationStartedString(i)); },
      [&logger, &graphs](int i, uni_course_cpp::Graph graph) {
        auto const graphDescription =
            uni_course_cpp::GraphPrinter::printGraph(graph);
        logger.log(generationFinishedString(i, graphDescription));
        const auto graphPrinter = uni_course_cpp::GraphJsonPrinter(graph);
        writeToFile(graphPrinter.print(),
                    "graph_" + std::to_string(i) + ".json");
        graphs.push_back(std::move(graph));
      });
  return graphs;
}

int main() {
  int const depth = intInput("depth: ");
  int const newVerticesNum = intInput("new_vertex_num: ");
  int const graphsCount = intInput("graphs_count: ");
  int const threadNum = intInput("thread_num: ");
  prepareTempDirectory();
  auto const params =
      uni_course_cpp::GraphGenerator::Params(depth, newVerticesNum);
  const auto graphs = generateGraphs(params, graphsCount, threadNum);
  return 0;
}
