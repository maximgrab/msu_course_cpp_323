#include "graph_printer.hpp"
#include <sstream>
#include <string>
namespace {
using uni_course_cpp::Edge;
using uni_course_cpp::Graph;
using uni_course_cpp::Vertex;

std::string edge_color_to_string(const Edge::Color& color) {
  switch (color) {
    case Edge::Color::Gray:
      return "gray";
    case Edge::Color::Green:
      return "green";
    case Edge::Color::Red:
      return "red";
    case Edge::Color::Blue:
      return "blue";
    case Edge::Color::Yellow:
      return "yellow";
  }
  throw std::runtime_error("Color doesn't exist");
}

std::string edge_to_string(const Edge& edge) {
  std::stringstream buffer;
  buffer << "{\"id\":" << edge.id << ",\"vertex_ids\":["
         << edge.vertex_ids.first << "," << edge.vertex_ids.second
         << "],\"color\":\"" << edge_color_to_string(edge.color) << "\"}";
  return buffer.str();
}

std::string vertex_to_string(const Vertex& vertex) {
  std::stringstream buffer;
  buffer << "{\"id\":" << vertex.id << ",\"edge_ids\":[";
  const auto& edge_ids = vertex.get_edge_ids();
  for (int i = 0; i < edge_ids.size() - 1; i++)
    buffer << edge_ids[i] << ",";
  buffer << edge_ids[edge_ids.size() - 1] << "],\"depth\":" << vertex.depth
         << "}";
  return buffer.str();
}
}  // namespace
namespace uni_course_cpp {
std::string GraphPrinter::print() const {
  std::stringstream buffer;
  const auto& vertices = graph_.get_vertices();
  const auto& edges = graph_.get_edges();
  buffer << "{\"depth\":" << graph_.get_depth() << ",\"vertices\":[";
  for (unsigned int i = 0; i < vertices.size(); i++) {
    const auto& vertex = vertices[i];
    buffer << vertex_to_string(vertex);
    if (i != vertices.size() - 1)
      buffer << ",";
  }
  buffer << "],\"edges\":[";
  for (unsigned int i = 0; i < edges.size(); i++) {
    const auto& edge = edges[i];
    buffer << edge_to_string(edge);
    if (i != edges.size() - 1)
      buffer << ",";
  }
  buffer << "]}\n";
  return buffer.str();
}
}  // namespace uni_course_cpp