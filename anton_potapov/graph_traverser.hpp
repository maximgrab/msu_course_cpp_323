#pragma once

#include <map>
#include "graph.hpp"

namespace uni_cource_cpp {
class GraphTraverser {
 public:
  static std::map<VertexId, int> dynamic_bfs(const Graph& graph,
                                             int start_depth);
  int get_vertex_depth(const Graph& Graph, VertexId vertex_id, int start_depth);
};
}  // namespace uni_cource_cpp
