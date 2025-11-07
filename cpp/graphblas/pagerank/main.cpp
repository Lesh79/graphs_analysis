#include <LAGraph.h>
#include <LAGraphX.h>
#include <iostream>

#include "algo.h"
#include "parser.h"
#include "pr_config.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_configs_dir>\n";
        return 1;
    }

    GrB_init(GrB_NONBLOCKING);
    LAGraph_Init(NULL);

    PRConfig config = PRConfig::Parse(std::string(argv[1]) + "/pr.ini");

    Parser parser;
    GBGraph graph = parser.ParseSNAP(config.GetGraphPath());

    GBPageRank pagerank(config.GetDampingFactor(), config.GetTolerance(), 100);
    pagerank.RunAlgo(graph);

    std::cout << pagerank.GetExecTime().count() << std::endl;

    GrB_Matrix_free(&graph.matrix);
    LAGraph_Finalize(NULL);
    GrB_finalize();

    return 0;
}