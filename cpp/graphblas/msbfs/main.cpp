#include <GraphBLAS.h>
#include <iostream>

#include "algo.h"
#include "msbfs_config.h"
#include "parser.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_config>\n";
        return 1;
    }

    GrB_init(GrB_NONBLOCKING);

    MSBFSConfig config = MSBFSConfig::Parse(std::string(argv[1]));

    Parser parser;
    GBGraph graph = parser.ParseDIMACS(config.GetGraphPath(), false);

    GBMSBFS bfs(config.GetStartingVertices());
    bfs.RunAlgo(graph);

    std::cout << bfs.GetExecTime().count() << std::endl;

    GrB_Matrix_free(&graph.matrix);
    GrB_finalize();

    return 0;
}
