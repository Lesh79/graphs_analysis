#pragma once

#include <fstream>
#include <sstream>
#include <string>

#include "config.h"

class SSBFSConfig : public Config {
private:
    int starting_vertex_;

public:
    SSBFSConfig(std::string graph_path, int starting_vertex = 0)
        : Config(graph_path), starting_vertex_(starting_vertex) {}

    int GetStartingVertex() const {
        return starting_vertex_;
    }

    static SSBFSConfig Parse(std::string const& config_path) {
        std::ifstream fin(config_path);
        if (!fin.is_open()) {
            throw std::runtime_error("Failed to open file " + config_path);
        }

        std::string line;
        std::string graph_path;
        int starting_vertex = -1;

        while (std::getline(fin, line)) {
            line = Trim(line);
            if (line.empty() || line[0] == '#') {
                continue;
            }

            size_t delimiter_pos = line.find('=');
            if (delimiter_pos == std::string::npos) {
                continue;
            }

            std::string key = Trim(line.substr(0, delimiter_pos));
            std::string value = Trim(line.substr(delimiter_pos + 1));

            if (key == "GraphPath") {
                graph_path = value;
            } else if (key == "StartingVertex") {
                starting_vertex = std::stoi(value);
            } else {
                throw std::runtime_error("Wrong key: " + key);
            }
        }

        if (graph_path.empty()) {
            throw std::runtime_error("GraphPath not specified in config: " + config_path);
        }
        if (starting_vertex == -1) {
            throw std::runtime_error("StartingVertex not specified in config: " + config_path);
        }

        return SSBFSConfig(graph_path, starting_vertex);
    }
};
