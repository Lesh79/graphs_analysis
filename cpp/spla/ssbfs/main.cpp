#include <iostream>
#include <spla.hpp>

#include "parser.h"
#include "ssbfs.h"
#include "ssbfs_config.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_config>\n";
        return 1;
    }

    SSBFSConfig config = SSBFSConfig::Parse(std::string(argv[1]));
    Parser parser;
    SPLAGraph graph = parser.ParseDIMACS(config.GetGraphPath(), false);
    SSBFSRunner runner(config.GetStartingVertex());
    auto time = runner.RunAlgo(graph);
    std::cout << time << std::endl;
    return 0;
}