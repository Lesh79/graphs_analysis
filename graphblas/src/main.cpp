#include <GraphBLAS.h>

#include <iostream>

#include "algos/boruvka.h"
#include "parser/parser.h"

int main(int argc, char** argv) {
    GrB_init(GrB_NONBLOCKING);

    Graph graph = GraphParser::ParseDIMACS("../data/USA-road-d.NY.gr");
    std::cout << "Graph: " << graph.n_nodes << ' ' << graph.n_arcs << std::endl;

    BoruvkaRunner runner;
    int status = runner.RunAlgo(graph);
    std::cout << "Status: " << status << std::endl;

    return 0;
}