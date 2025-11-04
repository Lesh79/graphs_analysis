#include <GraphBLAS.h>
#include <iostream>
#include <limits>

#include "algo.h"
#include "graph.h"
#include "msbfs_config.h"
#include "parser.h"

int main() {
    GrB_init(GrB_NONBLOCKING);

    MSBFSConfig config =
            MSBFSConfig::Parse("/home/maybenotilya/proj/graphs_analysis/configs/msbfs.ini");

    Parser parser;
    GBGraph graph = parser.ParseDIMACS(config.GetGraphPath(), false);

    GBMSBFS bfs(config.GetStartingVertices());

    bfs.RunAlgo(graph);

    std::cout << bfs.GetExecTime().count() << std::endl;

    GrB_Matrix_free(&graph.matrix);
    GrB_finalize();

    return 0;
}
