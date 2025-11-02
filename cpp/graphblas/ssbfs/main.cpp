#include <iostream>
#include "parser.h"

int main() {
    Parser parser;
    GBGraph graph = parser.ParseDIMACS(
        "/home/maybenotilya/proj/graphs_analysis/data/NY.gr");
    std::cout << graph.n_arcs << ' ' << graph.n_nodes << std::endl;
    return 0;
}