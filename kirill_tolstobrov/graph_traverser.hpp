#pragma once

#include <vector>

#include "graph.hpp"
#include "graph_path.hpp"

namespace uni_cpp_practice {

class GraphTraverser {
 public:
  explicit GraphTraverser(const Graph& graph) : graph_(graph){};

  struct VertexInfo {
    VertexId nearest_neighbour;
    GraphPath::Distance distance;
    bool visited;
  };

  VertexId next_vertex_to_check(
      std::map<VertexId, VertexInfo>& vertices_info) const;
  GraphPath find_shortest_path(const VertexId& source_verex_id,
                               const VertexId& destination_vertex_id) const;

  std::vector<GraphPath> find_all_paths() const;

 private:
  const Graph& graph_;
};

}  // namespace uni_cpp_practice
