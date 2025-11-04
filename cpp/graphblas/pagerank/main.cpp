#include <LAGraph.h>
#include <LAGraphX.h>
#include <iostream>

#include "algo.h"
#include "parser.h"

int main() {
    GrB_init(GrB_NONBLOCKING);

    LAGraph_Init(NULL);

    GBGraph graph;
    graph.n_nodes = 4;
    GrB_Matrix_new(&graph.matrix, GrB_FP64, graph.n_nodes, graph.n_nodes);

    auto add_edge = [&](GrB_Index from, GrB_Index to) {
        GrB_Matrix_setElement_FP64(graph.matrix, 1.0, from, to);
    };

    add_edge(0, 1);
    add_edge(0, 2);
    add_edge(1, 2);
    add_edge(2, 0);
    add_edge(2, 3);
    add_edge(3, 0);

    graph.is_inited = true;

    GBPageRank pagerank(0.85, 1e-4, 100);
    pagerank.RunAlgo(graph);

    std::cout << "PageRank result: ";
    double s = 0.0;
    for (auto const& val : pagerank.GetResult()) {
        s += val;
        std::cout << val << ' ';
    }
    std::cout << s << std::endl;
    std::cout << std::endl;

    return 0;
}