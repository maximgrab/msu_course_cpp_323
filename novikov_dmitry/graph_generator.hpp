#pragma once

#include "graph.hpp"

namespace uni_cpp_practice {

class GraphGenerator {
 public:
  struct Params {
    explicit Params(Depth _depth = 0, int _new_vertices_num = 0)
        : depth(_depth), new_vertices_num(_new_vertices_num) {}

    const Depth depth = 0;
    const int new_vertices_num = 0;
  };

  explicit GraphGenerator(const Params& params = Params()) : params_(params) {}

  Graph generate() const;

 private:
  const Params params_ = Params();
};
}  // namespace uni_cpp_practice