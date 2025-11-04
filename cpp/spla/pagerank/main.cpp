#include <iostream>
#include <spla.hpp>

#include "parser.h"
#include "pr.h"
#include "pr_config.h"

int main() {
    PRConfig config = PRConfig::Parse("/home/maybenotilya/proj/graphs_analysis/configs/pr.ini");
    Parser parser;
    auto [graph, indices] =
            parser.ParseSNAP("/home/maybenotilya/proj/graphs_analysis/data/web-Stanford.gr");
    PageRankRunner runner(indices, config.GetDampingFactor(), config.GetTolerance());
    auto time = runner.RunAlgo(graph);
    std::cout << time << std::endl;
    runner.GetResult();
    return 0;
}