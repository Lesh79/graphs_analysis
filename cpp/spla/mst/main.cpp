#include <iostream>
#include <spla.hpp>

#include "mst.h"
#include "mst_config.h"
#include "parser.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_config>\n";
        return 1;
    }

    MSTConfig config = MSTConfig::Parse(std::string(argv[1]));
    Parser parser;
    SPLAGraph graph = parser.ParseDIMACS(config.GetGraphPath(), false);
    MSTRunner runner;
    auto time = runner.RunAlgo(graph);
    std::cout << time << std::endl;
    return 0;
}