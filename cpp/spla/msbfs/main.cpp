#include <iostream>
#include <spla.hpp>
#include <string.h>

#include "msbfs.h"
#include "msbfs_config.h"
#include "parser.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0]
                  << " <path_to_config> <\"acc\" | \"noacc\" | \"pocl\" | \"intel\">* \n";
        return 1;
    }

    auto lib = spla::Library::get();

    for (int i = 2; i < argc; ++i) {
        if (strcmp(argv[i], "pocl") == 0) {
            lib->set_platform(1);
            lib->set_device(0);
            std::clog << "[SPLA MST LOG] Set platform to POCL" << std::endl;
        } else if (strcmp(argv[i], "intel") == 0) {
            lib->set_platform(1);
            lib->set_device(0);
            std::clog << "[SPLA MST LOG] Set platform to Intel OCL runtime" << std::endl;
        } else if (strcmp(argv[i], "acc") == 0) {
            lib->set_force_no_acceleration(false);
            std::clog << "[SPLA MST LOG] Set accelerator to use" << std::endl;
        } else if (strcmp(argv[i], "noacc") == 0) {
            lib->set_force_no_acceleration(true);
            std::clog << "[SPLA MST LOG] Set cpu to use" << std::endl;
        } else {
            std::clog << "[SPLA MST LOG] Unknown CLI param" << std::endl;
        }
    }

    std::string info;
    lib->get_accelerator_info(info);
    std::clog << "[SPLA MST LOG] env: " << info << std::endl;

    MSBFSConfig config = MSBFSConfig::Parse(std::string(argv[1]));
    Parser parser;
    SPLAGraph graph = parser.ParseDIMACS(config.GetGraphPath(), false);
    MSBFSRunner runner(config.GetStartingVertices());
    auto time = runner.RunAlgo(graph);
    std::cout << time << std::endl;
    return 0;
}