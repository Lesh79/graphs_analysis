#include "parser.h"

#include "graph.h"

GBGraph Parser::ParseDIMACS(std::string const& filepath, bool weights) {
    GBGraph graph;

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

            int status = GrB_Matrix_new(&graph.matrix, GrB_UINT64, graph.n_nodes, graph.n_nodes);
            graph.is_inited = true;
        }

        if (type == 'a') {
            GrB_Index src, dest;
            int weight;
            iss >> src >> dest >> weight;
            if (!graph.is_inited) {
                throw std::runtime_error("Graph is not inited");
            }

            if (weight) {
                GrB_Matrix_setElement_UINT64(graph.matrix, weight, src - 1, dest - 1);
                GrB_Matrix_setElement_UINT64(graph.matrix, weight, dest - 1, src - 1);
            } else {
                GrB_Matrix_setElement_UINT64(graph.matrix, 1, src - 1, dest - 1);
                GrB_Matrix_setElement_UINT64(graph.matrix, 1, dest - 1, src - 1);
            }
        }
    }

    return graph;
}

GBGraph Parser::ParseSNAP(std::string const& filepath) {
    GBGraph graph;
    std::ifstream f(filepath);
    if (!f.is_open()) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }

    std::string line;
    GrB_Index n_nodes = 0;
    size_t n_edges = 0;

    while (std::getline(f, line)) {
        if (line.rfind("# Nodes:", 0) == 0) {
            std::stringstream ss(line);
            std::string tmp;
            ss >> tmp >> tmp >> n_nodes >> tmp >> n_edges;
            break;
        }
    }

    if (n_nodes == 0) {
        throw std::runtime_error("Failed to determine node count from SNAP header");
    }

    std::vector<GrB_Index> I, J;
    std::vector<uint64_t> X;
    I.reserve(n_edges);
    J.reserve(n_edges);
    X.reserve(n_edges);

    while (std::getline(f, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream iss(line);
        GrB_Index src, dest;
        if (!(iss >> src >> dest)) continue;

        I.push_back(src - 1);
        J.push_back(dest - 1);
        X.push_back(1);
    }

    GrB_Matrix_new(&graph.matrix, GrB_UINT64, n_nodes, n_nodes);
    GrB_Matrix_build_UINT64(graph.matrix, I.data(), J.data(), X.data(), I.size(),
                            GrB_SECOND_UINT64);

    graph.n_nodes = n_nodes;
    graph.n_arcs = n_edges;
    graph.is_inited = true;
    return graph;
}
