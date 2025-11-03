#include <iostream>
#include <limits>
#include <GraphBLAS.h>
#include "graph.h"
#include "algo.h"

int main() {
    GrB_init(GrB_NONBLOCKING);

    GBGraph graph;
    graph.is_inited = true;
    graph.n_nodes = 4;
    GrB_Matrix_new(&graph.matrix, GrB_BOOL, graph.n_nodes, graph.n_nodes);

    GrB_Matrix_setElement_BOOL(graph.matrix, true, 0, 1);
    GrB_Matrix_setElement_BOOL(graph.matrix, true, 1, 2);
    GrB_Matrix_setElement_BOOL(graph.matrix, true, 2, 3);
    GrB_Matrix_setElement_BOOL(graph.matrix, true, 1, 0);

    std::vector<GrB_Index> sources = {0, 2};
    GBMSBFS bfs(sources);

    bfs.RunAlgo(graph);

    const auto& result = bfs.GetResult();

    std::cout << "=== Multiple-Source Parent Arrays ===\n";
    for (size_t s = 0; s < result.size(); ++s) {
        std::cout << "Source " << sources[s] << ":\n";
        for (size_t i = 0; i < result[s].size(); ++i) {
            if (result[s][i] == std::numeric_limits<GrB_Index>::max())
                std::cout << "  " << i << " -> unreachable\n";
            else
                std::cout << "  " << i << " <- " << result[s][i] << "\n";
        }
        std::cout << std::endl;
    }

    GrB_Matrix_free(&graph.matrix);
    GrB_finalize();

    return 0;
}
