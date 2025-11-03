#include <iostream>
#include <spla.hpp>

#include "msbfs.h"
#include "parser.h"

int main() {
    auto lib = spla::Library::get();

    Parser parser;
    SPLAGraph graph =
            parser.ParseDIMACS("/home/maybenotilya/proj/graphs_analysis/data/NY.gr", false);
    MSBFSRunner runner;
    std::set<int> ss{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    runner.SetStartingVertices(ss);
    auto time = runner.RunAlgo(graph);
    std::cout << time << std::endl;
    return 0;
}