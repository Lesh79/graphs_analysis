#include <iostream>
#include <spla.hpp>

#include "parser.h"
#include "pr.h"

int main() {
    Parser parser;
    auto [graph, indices] =
            parser.ParseSNAP("/home/maybenotilya/proj/graphs_analysis/data/web-Stanford.gr");
    PageRankRunner runner;
    runner.SetIndices(indices);
    auto time = runner.RunAlgo(graph);
    std::cout << time << std::endl;
    runner.GetResult();
    return 0;
}