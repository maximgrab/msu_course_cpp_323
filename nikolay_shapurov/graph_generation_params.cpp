#include "graph_generation_params.hpp"

Graph GraphGenerator::generate() const {
  std::random_device rd;
  std::mt19937 generator(rd());

  Graph graph;
  graph.add_vertex();
  const int next_vertices_depth = 1;
  build_graph(graph, graph.vertices().back().id, next_vertices_depth,
              generator);

  add_green_edges(graph, generator);
  add_yellow_edges(graph, generator);
  add_red_edges(graph, generator);

  return graph;
}

void GraphGenerator::build_graph(Graph& graph,
                                 VertexId parent_id,
                                 int current_layer,
                                 std::mt19937& generator) const {
  if (current_layer > params_.depth) {
    return;
  }

  std::vector<VertexId> vec =
      gen_layer(graph, parent_id, current_layer, generator);
  for (int id : vec) {
    build_graph(graph, id, current_layer + 1, generator);
  }
}

std::vector<VertexId> GraphGenerator::gen_layer(Graph& graph,
                                                VertexId parent_id,
                                                int current_layer,
                                                std::mt19937& generator) const {
  graph.set_adding_vertex_depth(current_layer);
  std::vector<VertexId> child_ids;
  for (int attempt = 0; attempt < params_.new_vertices_num; attempt++) {
    if (gen_attempt((params_.depth - current_layer + 1), params_.depth,
                    generator)) {
      graph.add_vertex();
      VertexId child_id = graph.vertices().back().id;
      child_ids.emplace_back(child_id);
      graph.add_edge(parent_id, child_id);
    }
  }

  return child_ids;
}

bool GraphGenerator::gen_attempt(int positive_outcomes,
                                 int attempts_num,
                                 std::mt19937& generator) const {
  std::bernoulli_distribution d(static_cast<double>(positive_outcomes) /
                                attempts_num);
  return d(generator);
}

void GraphGenerator::add_green_edges(Graph& graph,
                                     std::mt19937& generator) const {
  const int successful_case = 1;
  const int total_case = 10;
  for (const Vertex& vertex : graph.vertices()) {
    if (gen_attempt(successful_case, total_case, generator)) {
      graph.add_edge(vertex.id, vertex.id);
    }
  }
}

void GraphGenerator::add_yellow_edges(Graph& graph,
                                      std::mt19937& generator) const {
  const int diff_depth = 1;
  const int graph_depth = graph.depth();
  for (const Vertex& from_vertex : graph.vertices()) {
    if (from_vertex.depth < graph_depth) {
      if (gen_attempt(from_vertex.depth, params_.depth - diff_depth,
                      generator)) {
        VertexId to_vertex_id;
        if (get_vertex_id_on_distance(graph, from_vertex, diff_depth,
                                      to_vertex_id)) {
          graph.add_edge(from_vertex.id, to_vertex_id);
        }
      }
    }
  }
}

void GraphGenerator::add_red_edges(Graph& graph,
                                   std::mt19937& generator) const {
  const int successful_case = 1;
  const int total_case = 3;
  const int diff_depth = 2;
  for (const Vertex& from_vertex : graph.vertices()) {
    if (gen_attempt(successful_case, total_case, generator)) {
      VertexId to_vertex_id;
      if (get_vertex_id_on_distance(graph, from_vertex, diff_depth,
                                    to_vertex_id)) {
        graph.add_edge(from_vertex.id, to_vertex_id);
      }
    }
  }
}

bool GraphGenerator::get_vertex_id_on_distance(Graph& graph,
                                               const Vertex& from_vertex,
                                               int distance,
                                               VertexId& to_vertex_id) const {
  for (const Vertex& to_vertex : graph.vertices()) {
    if (((from_vertex.depth + distance) == to_vertex.depth) &&
        !graph.has_edge(from_vertex.id, to_vertex.id)) {
      to_vertex_id = to_vertex.id;
      return true;
    }
  }
  return false;
}
