#include "parser/parser.h"

#include <fstream>
#include <iostream>
#include <sstream>

Graph GraphParser::ParseDIMACS(std::string const& filepath) {
    Graph graph;
    graph.is_inited = false;
    std::ifstream f(filepath);
    if (!f.is_open()) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }
    std::string line;
    while (std::getline(f, line)) {
        if (line.empty()) {
            continue;
        }

        std::istringstream iss(line);
        char type;
        iss >> type;
        if (type == 'c') {
            continue;
        }

        if (type == 'p') {
            std::string op;
            iss >> op >> graph.n_nodes >> graph.n_arcs;

            int status = GrB_Matrix_new(&graph.matrix, GrB_UINT64,
                                        graph.n_nodes, graph.n_nodes);
            std::cout << status;
            graph.is_inited = true;
        }

        if (type == 'a') {
            GrB_Index src, dest;
            double weight;
            iss >> src >> dest >> weight;
            if (!graph.is_inited) {
                throw std::runtime_error("Graph is not inited");
            }

            GrB_Matrix_setElement_UINT64(graph.matrix, weight, src - 1,
                                         dest - 1);
            GrB_Matrix_setElement_UINT64(graph.matrix, weight, dest - 1,
                                         src - 1);
        }
    }

    return graph;
}