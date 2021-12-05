#pragma once

#include <string>
#include "graph.hpp"

namespace uni_cource_cpp {

class GraphPrinter {
 public:
  GraphPrinter(const Graph& graph);
  std::string print() const;

  std::string print_vertex(const Vertex& vertex) const;
  std::string print_edge(const Edge& edge) const;

 private:
  const Graph& graph_;
};

}  // namespace uni_cource_cpp
