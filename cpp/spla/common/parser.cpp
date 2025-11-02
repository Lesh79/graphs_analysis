#include "parser.h"

#define SPLA_OK(call)                                         \
    do {                                                      \
        auto status = (call);                                 \
        if (status != spla::Status::Ok) {                     \
            throw std::runtime_error("Exited with status " +  \
                                     std::to_string(status)); \
        }                                                     \
    } while (0)

SPLAGraph Parser::ParseDIMACS(std::string const& filepath) {
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
            graph.matrix = spla::Matrix::make(graph.n_vertices,
                                              graph.n_vertices, spla::INT);
            graph.matrix->set_fill_value(spla::Scalar::make_int(0));
        }

        if (type == 'a') {
            int src, dest;
            int weight;
            iss >> src >> dest >> weight;

            graph.matrix->set_int(src - 1, dest - 1, weight);
            graph.matrix->set_int(dest - 1, src - 1, weight);
        }
    }

    return graph;
}