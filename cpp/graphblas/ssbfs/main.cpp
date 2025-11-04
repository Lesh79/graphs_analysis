#include <iostream>

#include "algo.h"
#include "msbfs_config.h"
#include "parser.h"

int main() {
    GrB_init(GrB_NONBLOCKING);

    MSBFSConfig config =
            MSBFSConfig::Parse("/home/maybenotilya/proj/graphs_analysis/configs/msbfs.ini");

    Parser parser;
    GBGraph graph = parser.ParseDIMACS(config.GetGraphPath(), false);

    GBSSBFS bfs(config.GetStartingVertices().front());
    bfs.RunAlgo(graph);

    std::cout << bfs.GetExecTime().count() << std::endl;

    auto const& parents = bfs.GetResult();

    GrB_Matrix_free(&graph.matrix);
    GrB_finalize();
    return 0;
}
