#include "graph.hpp"
#include <algorithm>
#include <cassert>
#include <stack>

void Graph::add_vertex() {
  const VertexId new_vertex_id = get_new_vertex_id();
  vertices_.emplace_back(new_vertex_id, adding_vertex_depth_);
  adjacency_map_[new_vertex_id] = {};
}

void Graph::add_edge(const VertexId& from_vertex_id,
                     const VertexId& to_vertex_id) {
  assert((from_vertex_id >= 0) &&
         "The from_vertex_id should be greater or equal to 0.");
  assert((to_vertex_id >= 0) &&
         "The to_vertex_id should be greater or equal to 0.");

  assert(has_vertex(from_vertex_id) && "Vertex doesn't exist");
  assert(has_vertex(to_vertex_id) && "Vertex doesn't exist");

  assert(!has_edge(from_vertex_id, to_vertex_id) &&
         "Vertices are already connected");

  const Vertex& from_vertex = get_vertex(from_vertex_id);
  const Vertex& to_vertex = get_vertex(to_vertex_id);
  const int new_edge_id = get_new_edge_id();
  Edge::Color edge_color = Edge::Color::Grey;
  if (from_vertex_id == to_vertex_id)
    edge_color = Edge::Color::Green;
  else if (!adjacency_map_.at(to_vertex_id).empty() &&
           (abs(to_vertex.depth - from_vertex.depth) == 1))
    edge_color = Edge::Color::Yellow;
  else if (abs(to_vertex.depth - from_vertex.depth) == 2)
    edge_color = Edge::Color::Red;

  edges_.emplace_back(new_edge_id, from_vertex_id, to_vertex_id, edge_color);

  adjacency_map_[from_vertex_id].emplace_back(new_edge_id);
  adjacency_map_[to_vertex_id].emplace_back(new_edge_id);
}

const std::vector<EdgeId>& Graph::get_edge_ids(const VertexId& id) const {
  return adjacency_map_.at(id);
}

int Graph::depth() const {
  int depth = 0;
  for (const Vertex& v : vertices_) {
    if (depth < v.depth) {
      depth = v.depth;
    }
  }
  return depth;
}

bool Graph::has_vertex(const VertexId& id) const {
  for (const Vertex& vertex : vertices_) {
    if (id == vertex.id) {
      return true;
    }
  }
  return false;
}

bool Graph::has_edge(const VertexId& from_vertex_id,
                     const VertexId& to_vertex_id) const {
  if (from_vertex_id == to_vertex_id) {
    for (const auto& from_edge_id : adjacency_map_.at(from_vertex_id)) {
      for (const auto& to_edge_id : adjacency_map_.at(to_vertex_id)) {
        const Edge& to_edge = get_edge(to_edge_id);
        if (to_edge.from_vertex_id == to_edge.to_vertex_id &&
            from_edge_id == to_edge.from_vertex_id) {
          return true;
        }
      }
    }
  } else {
    for (const auto& from_edge_id : adjacency_map_.at(from_vertex_id)) {
      for (const auto& to_edge_id : adjacency_map_.at(to_vertex_id)) {
        if (from_edge_id == to_edge_id) {
          return true;
        }
      }
    }
  }

  return false;
}

bool Graph::has_edge(const EdgeId& id) const {
  for (const Edge& edge : edges_) {
    if (id == edge.id) {
      return true;
    }
  }
  return false;
}

const Vertex& Graph::get_vertex(const VertexId& id) const {
  assert(has_vertex(id) && "Vertex doesn't exist");
  const auto it = std::find_if(vertices_.begin(), vertices_.end(),
                               [id](const Vertex& v) { return v.id == id; });
  return *it;
}

const Edge& Graph::get_edge(const EdgeId& id) const {
  assert(has_edge(id) && "Edge doesn't exist");
  const auto it = std::find_if(edges_.begin(), edges_.end(),
                               [id](const Edge& e) { return e.id == id; });
  return *it;
}

int Graph::get_vertex_depth(const VertexId& vertex_id) const {
  return get_vertex(vertex_id).depth;
}

void Graph::set_adding_vertex_depth(const int depth) {
  adding_vertex_depth_ = depth;
}

Edge::Color Graph::get_edge_color(const EdgeId& edge_id) const {
  const Edge& edge = get_edge(edge_id);
  return edge.color;
}
