#include <iostream>
#include <spla.hpp>

#include "mst.h"
#include "mst_config.h"
#include "parser.h"

int main() {
    MSTConfig config = MSTConfig::Parse("/home/maybenotilya/proj/graphs_analysis/configs/mst.ini");
    Parser parser;
    SPLAGraph graph = parser.ParseDIMACS(config.GetGraphPath(), false);
    MSTRunner runner;
    auto time = runner.RunAlgo(graph);
    std::cout << time << std::endl;
    return 0;
}