#pragma once

#include "graph.hpp"

#include <random>

class GraphGenerator {
 public:
  struct Params {
    explicit Params(int _depth = 0, int _new_vertices_num = 0)
        : depth(_depth), new_vertices_num(_new_vertices_num) {}

    const int depth = 0;
    const int new_vertices_num = 0;
  };

  explicit GraphGenerator(const Params& params = Params()) : params_(params) {}

  Graph generate() const;

 private:
  void build_graph(Graph& graph,
                   VertexId parent_id,
                   int current_layer,
                   std::mt19937& generator) const;
  std::vector<VertexId> gen_layer(Graph& graph,
                                  VertexId parent_id,
                                  int current_layer,
                                  std::mt19937& generator) const;
  bool gen_attempt(int positive_outcomes,
                   int attempts_num,
                   std::mt19937& generator) const;
  void add_green_edges(Graph& graph, std::mt19937& generator) const;
  void add_yellow_edges(Graph& graph, std::mt19937& generator) const;
  void add_red_edges(Graph& graph, std::mt19937& generator) const;
  bool get_vertex_id_on_distance(Graph& graph,
                                 const Vertex& from_vertex,
                                 int distance,
                                 VertexId& to_vertex_id) const;

  const Params params_ = Params();
};
