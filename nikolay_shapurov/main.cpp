#include "graph.hpp"
#include "graph_generation_params.hpp"
#include "graph_printer.hpp"

#include <fstream>
#include <iostream>

void write_to_file(const std::string& graph_json,
                   const std::string& file_name) {
  std::ofstream ofs(file_name);
  if (ofs.is_open()) {
    ofs << graph_json;
  }
  ofs.close();
}

int handle_depth_input() {
  while (true) {
    std::cout << "Enter graph max depth (0..): ";
    int depth = 0;
    std::cin >> depth;
    if (depth >= 0)
      return depth;
    std::cout << " - wrong graph max depth - " << depth << std::endl;
  }
}

int handle_new_vertices_num_input() {
  while (true) {
    std::cout << "Enter graph new vertices number (0..): ";
    int new_vertices_num = 0;
    std::cin >> new_vertices_num;
    if (new_vertices_num >= 0)
      return new_vertices_num;
    std::cout << " - wrong graph new vertices number - " << new_vertices_num
              << std::endl;
  }
}

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_num = handle_new_vertices_num_input();

  const auto params = GraphGenerator::Params(depth, new_vertices_num);
  const auto generator = GraphGenerator(params);
  const auto graph = generator.generate();
  const auto graph_printer = GraphPrinter(graph);
  const auto graph_json = graph_printer.print();

  std::cout << graph_json << std::endl;
  write_to_file(graph_json, "graph.json");

  return 0;
}
