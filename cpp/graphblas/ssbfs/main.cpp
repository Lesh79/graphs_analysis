#include "parser.h"
#include "algo.h"

#include <iostream>

int main() {
    GrB_init(GrB_NONBLOCKING);

    GBGraph graph;
    graph.is_inited = true;
    GrB_Matrix_new(&graph.matrix, GrB_BOOL, 4, 4);
    graph.n_nodes = 4;

    GrB_Matrix_setElement_BOOL(graph.matrix, true, 0, 1);
    GrB_Matrix_setElement_BOOL(graph.matrix, true, 1, 2);
    GrB_Matrix_setElement_BOOL(graph.matrix, true, 2, 3);
    GrB_Matrix_setElement_BOOL(graph.matrix, true, 1, 0);

    GBSSBFS bfs(0);
    bfs.RunAlgo(graph);

    auto const& parents = bfs.GetResult();

    std::cout << "Parent array:\n";
    for (size_t i = 0; i < parents.size(); ++i) {
        if (parents[i] == std::numeric_limits<GrB_Index>::max())
            std::cout << i << " -> unreachable\n";
        else
            std::cout << i << " <- " << parents[i] << "\n";
    }

    GrB_Matrix_free(&graph.matrix);
    GrB_finalize();
    return 0;
}
