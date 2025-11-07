#include <iostream>
#include <spla.hpp>

#include "parser.h"
#include "pr.h"
#include "pr_config.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_configs_dir>\n";
        return 1;
    }

    PRConfig config = PRConfig::Parse(std::string(argv[1]) + "/pr.ini");
    Parser parser;
    auto [graph, indices] =
            parser.ParseSNAP(config.GetGraphPath());
    PageRankRunner runner(indices, config.GetDampingFactor(), config.GetTolerance());
    auto time = runner.RunAlgo(graph);
    std::cout << time << std::endl;
    runner.GetResult();
    return 0;
}