#include <iostream>
#include <spla.hpp>

#include "msbfs.h"
#include "msbfs_config.h"
#include "parser.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_configs_dir>\n";
        return 1;
    }

    MSBFSConfig config =
        MSBFSConfig::Parse(std::string(argv[1]) + "/msbfs.ini");
    Parser parser;
    SPLAGraph graph = parser.ParseDIMACS(config.GetGraphPath(), false);
    MSBFSRunner runner(config.GetStartingVertices());
    auto time = runner.RunAlgo(graph);
    std::cout << time << std::endl;
    return 0;
}