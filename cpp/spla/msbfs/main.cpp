#include <iostream>
#include <spla.hpp>

#include "parser.h"

int main() {
    Parser parser;
    SPLAGraph graph = parser.ParseDIMACS(
        "/home/maybenotilya/proj/graphs_analysis/data/NY.gr");
    std::cout << graph.n_edges << ' ' << graph.n_vertices << std::endl;
    return 0;
}