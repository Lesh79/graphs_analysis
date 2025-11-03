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

    GBMST mst;
    mst.RunAlgo(graph);
    std::cout << "MST total weight: " << mst.GetResult().total_weight << std::endl;

    return 0;
}
