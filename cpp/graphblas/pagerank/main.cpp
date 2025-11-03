#include "algo.h"
#include <iostream>
#include <LAGraph.h>
#include <LAGraphX.h>

#include "parser.h"

int main() {
    GrB_init(GrB_NONBLOCKING);

    Parser parser;
    GBGraph graph = parser.ParseDIMACS("/Users/rafik/Desktop/USA-road-d.NY.gr");
    std::cout << "Graph: " << graph.n_nodes << ' ' << graph.n_arcs << std::endl;

    GBPageRank pagerank;
    pagerank.RunAlgo(graph);
    std::cout << "PageRank result: ";
    for (const auto& val : pagerank.GetResult()) {
        std::cout << val << ' ';
    }
    std::cout << std::endl;

    return 0;
}
