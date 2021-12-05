#include "graph.hpp"
#include "graph_generation_params.hpp"
#include "graph_printer.hpp"
#include "logger.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>

using namespace std;
using namespace uni_cource_cpp;

namespace fs = std::filesystem;

namespace {

const std::string result_files_dir_name = "temp/";

void write_to_file(const std::string& graph_json,
                   const std::string& file_name) {
  std::ofstream ofs(result_files_dir_name + file_name);
  if (ofs.is_open()) {
    ofs << graph_json;
  }
  ofs.close();
}

int handle_depth_input() {
  while (1) {
    std::cout << "Enter graph max depth (0..): ";
    int depth = 0;
    std::cin >> depth;
    if (depth >= 0)
      return depth;
    std::cout << " - wrong graph max depth - " << depth << std::endl;
  }
}

int handle_graphs_count_input() {
  while (1) {
    std::cout << "Enter graph count (1..): ";
    int count = 0;
    std::cin >> count;
    if (count > 0)
      return count;
    std::cout << " - wrong graph count - " << count << std::endl;
  }
}

int handle_new_vertices_num_input() {
  while (1) {
    std::cout << "Enter graph new vertices number (0..): ";
    int new_vertices_num = 0;
    std::cin >> new_vertices_num;
    if (new_vertices_num >= 0)
      return new_vertices_num;
    std::cout << " - wrong graph new vertices number - " << new_vertices_num
              << std::endl;
  }
}

std::string gen_started_string(int i) {
  return "Graph " + std::to_string(i) + ", Generation Started";
}

std::string get_graph_vetecies(const std::vector<Vertex>& vertices) {
  std::string str;
  str += std::to_string(vertices.size()) + " [";
  for (const Vertex& vertex : vertices) {
    str += std::to_string(vertex.id) + ", ";
  }
  if (!vertices.empty()) {
    str.pop_back();
    str.pop_back();
  }
  return str + "]";
}

std::string get_graph_edges(const std::vector<Edge>& edges) {
  std::string str;
  std::map<Edge::Color, int> color_counts = {{Edge::Color::Green, 0},
                                             {Edge::Color::Green, 0},
                                             {Edge::Color::Blue, 0},
                                             {Edge::Color::Yellow, 0},
                                             {Edge::Color::Red, 0}};
  str += std::to_string(edges.size()) + ", {";
  for (const Edge& edge : edges) {
    color_counts[edge.color] += 1;
  }
  str += "gray: " + std::to_string(color_counts.at(Edge::Color::Grey));
  str += ", green: " + std::to_string(color_counts.at(Edge::Color::Green));
  str += ", blue: " + std::to_string(color_counts.at(Edge::Color::Blue));
  str += ", yellow: " + std::to_string(color_counts.at(Edge::Color::Yellow));
  str += ", red: " + std::to_string(color_counts.at(Edge::Color::Red));
  return str;
}

std::string gen_finished_string(int i, const Graph& graph) {
  std::string str;
  str = "Graph " + std::to_string(i) + ", Generation Finished {\n";
  str += "depth: " + std::to_string(graph.get_graph_depth()) + ",\n";
  str += "vertices: ";
  str += get_graph_vetecies(graph.vertices());
  str += ",\nedges: ";
  str += get_graph_edges(graph.edges());
  return str + "},\n}";
}

Logger& prepare_logger() {
  if (!fs::exists(result_files_dir_name)) {
    fs::create_directory(result_files_dir_name);
  }
  Logger::set_log_path(result_files_dir_name);
  return Logger::get_instance();
}

}  // namespace

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_num = handle_new_vertices_num_input();
  const int graphs_count = handle_graphs_count_input();

  const auto params = GraphGenerator::Params(depth, new_vertices_num);
  const auto generator = GraphGenerator(params);
  auto& logger = prepare_logger();

  for (int i = 0; i < graphs_count; i++) {
    logger.log(gen_started_string(i));
    const auto graph = generator.generate();
    logger.log(gen_finished_string(i, graph));

    const auto graph_printer = GraphPrinter(graph);
    write_to_file(graph_printer.print(),
                  "graph_" + std::to_string(i) + ".json");
  }

  return 0;
}
