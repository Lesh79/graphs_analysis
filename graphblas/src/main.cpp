#include <GraphBLAS.h>

#include <iostream>

#include "algos/boruvka.h"
#include "parser/parser.h"

int main(int argc, char** argv) {
    // Graph graph = GraphParser::ParseDIMACS("data/USA-road-d.NY.gr");
    // std::cout << "Graph: " << graph.n_nodes << ' ' << graph.n_arcs <<
    // std::endl;

    GrB_Matrix A;
    int num_nodes = 10;
    GrB_Info status = GrB_Matrix_new(&A, GrB_UINT64, num_nodes, num_nodes);
    std::cout << status << std::endl;

    // BoruvkaRunner runner;
    // runner.RunAlgo(graph);

    return 0;
}