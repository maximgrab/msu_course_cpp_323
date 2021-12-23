#pragma once

#include <unordered_map>
#include <vector>

using VertexId = int;
using EdgeId = int;

struct Vertex {
  explicit Vertex(const VertexId& _id, const int _depth)
      : id(_id), depth(_depth) {}

  const VertexId id = 0;
  const int depth = 0;
};

struct Edge {
  enum class Color { Grey, Green, Yellow, Red };
  explicit Edge(const EdgeId& _id,
                const VertexId& _from_vertex_id,
                const VertexId& _to_vertex_id,
                const Color& _color = Color::Grey)
      : id(_id),
        from_vertex_id(_from_vertex_id),
        to_vertex_id(_to_vertex_id),
        color(_color) {}

  const EdgeId id = 0;
  const VertexId from_vertex_id = 0;
  const VertexId to_vertex_id = 0;
  const Color color = Color::Grey;
};

class Graph {
 public:
  void add_vertex();
  void add_edge(const VertexId& from_vertex_id, const VertexId& to_vertex_id);

  const std::vector<Vertex>& vertices() const { return vertices_; }
  const std::vector<Edge>& edges() const { return edges_; }
  const std::vector<EdgeId>& get_edge_ids(const VertexId& id) const;

  int depth() const;
  int get_vertex_depth(const VertexId& vertex_id) const;
  void set_adding_vertex_depth(const int depth);
  Edge::Color get_edge_color(const EdgeId& edge_id) const;
  bool has_edge(const VertexId& from_vertex_id,
                const VertexId& to_vertex_id) const;

 private:
  VertexId vertex_id_counter_ = 0;
  VertexId get_new_vertex_id() { return vertex_id_counter_++; }

  EdgeId edge_id_counter_ = 0;
  EdgeId get_new_edge_id() { return edge_id_counter_++; }

  bool has_vertex(const VertexId& id) const;

  bool has_edge(const EdgeId& id) const;

  const Vertex& get_vertex(const VertexId& id) const;
  const Edge& get_edge(const EdgeId& id) const;

 private:
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
  std::unordered_map<VertexId, std::vector<EdgeId>> adjacency_map_;
  int adding_vertex_depth_ = 0;
};
