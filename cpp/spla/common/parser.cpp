#include "parser.h"

#include <sstream>
#include <string>

SPLAGraph Parser::ParseDIMACS(std::string const& filepath, bool weights) {
    SPLAGraph graph;
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
            iss >> op >> graph.n_vertices >> graph.n_edges;
            graph.matrix = spla::Matrix::make(graph.n_vertices, graph.n_vertices, spla::INT);
            graph.matrix->set_fill_value(spla::Scalar::make_int(0));
        }

        if (type == 'a') {
            int src, dest;
            int weight;
            iss >> src >> dest >> weight;

            if (!weights) {
                weight = 1;
            }

            graph.matrix->set_int(src - 1, dest - 1, weight);
            graph.matrix->set_int(dest - 1, src - 1, weight);
        }
    }

    return graph;
}

std::pair<SPLAGraph, std::vector<std::pair<int, int>>> Parser::ParseSNAP(
        std::string const& filepath) {
    SPLAGraph graph;
    std::vector<std::pair<int, int>> indices;

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

        if (line.find("# Nodes:") != std::string::npos) {
            std::string tmp;
            iss >> tmp >> tmp >> graph.n_vertices >> tmp >> graph.n_edges;
            graph.matrix = spla::Matrix::make(graph.n_vertices, graph.n_vertices, spla::INT);
        }

        if (line[0] == '#') {
            continue;
        }

        int src, dest;
        iss >> src >> dest;
        if (src >= graph.n_vertices) {
            src = graph.n_vertices;
        }
        if (dest >= graph.n_vertices) {
            dest = graph.n_vertices;
        }
        if (src <= 0) {
            src = 1;
        }
        if (dest <= 0) {
            dest = 1;
        }

        indices.push_back({src - 1, dest - 1});
        graph.matrix->set_int(src - 1, dest - 1, 1);
    }

    return {graph, indices};
}