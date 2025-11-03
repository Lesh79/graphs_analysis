#include <iostream>
#include <spla.hpp>

#include "msbfs.h"
#include "parser.h"

int main() {
    Parser parser;
    SPLAGraph graph =
            parser.ParseDIMACS("/home/maybenotilya/proj/graphs_analysis/data/NY.gr", false);
    MSBFSRunner runner;
    std::set<int> ss{0};
    runner.SetStartingVertices(ss);
    auto time = runner.RunAlgo(graph);
    std::cout << time << std::endl;
    return 0;
}