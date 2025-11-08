#include <GraphBLAS.h>
#include <iostream>

#include "algo.h"
#include "parser.h"
#include "ssbfs_config.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_config>\n";
        return 1;
    }

    GrB_init(GrB_NONBLOCKING);

    SSBFSConfig config = SSBFSConfig::Parse(std::string(argv[1]));

    Parser parser;
    GBGraph graph = parser.ParseDIMACS(config.GetGraphPath(), false);

    GBSSBFS bfs(config.GetStartingVertex());
    bfs.RunAlgo(graph);

    std::cout << bfs.GetExecTime().count() << std::endl;

    GrB_Matrix_free(&graph.matrix);
    GrB_finalize();
    return 0;
}
