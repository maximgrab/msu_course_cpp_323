
clang-format -i -style=Chromium main.cpp
clang-format -i -style=Chromium graph.cpp
clang-format -i -style=Chromium graph.hpp
clang-format -i -style=Chromium graph_printer.cpp
clang-format -i -style=Chromium graph_printer.hpp
clang-format -i -style=Chromium graph_generation_params.hpp
clang-format -i -style=Chromium graph_generation_params.cpp

clang++ main.cpp  graph.cpp graph_printer.cpp graph_generation_params.cpp -o graph -std=c++17 -Werror

