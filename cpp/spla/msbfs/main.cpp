#include <iostream>
#include <spla.hpp>

#include "msbfs.h"
#include "msbfs_config.h"
#include "parser.h"

int main() {
    MSBFSConfig config =
            MSBFSConfig::Parse("/home/maybenotilya/proj/graphs_analysis/configs/msbfs.ini");
    Parser parser;
    SPLAGraph graph = parser.ParseDIMACS(config.GetGraphPath(), false);
    MSBFSRunner runner(config.GetStartingVertices());
    auto time = runner.RunAlgo(graph);
    std::cout << time << std::endl;
    return 0;
}