#pragma once

#include <fstream>
#include <sstream>
#include <vector>

#include "config.h"

class MSBFSConfig : public Config {
private:
    std::vector<int> starting_vertices_;

public:
    MSBFSConfig(std::string graph_path, std::vector<int> starting_vertices = {0})
        : Config(graph_path), starting_vertices_(starting_vertices) {}

    std::vector<int> const& GetStartingVertices() const {
        return starting_vertices_;
    }

    static MSBFSConfig Parse(std::string config_path) {
        std::ifstream fin(config_path);
        if (!fin.is_open()) {
            throw std::runtime_error("Failed to open file " + config_path);
        }
        std::string line;

        std::string graph_path;
        std::vector<int> starting_vertices;

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
            } else if (key == "StartingVertices") {
                std::stringstream ss(value);
                std::string s;
                while (std::getline(ss, s, ',')) {
                    s = Trim(s);
                    if (!s.empty()) {
                        starting_vertices.push_back(std::stoi(s));
                    }
                }
            } else {
                throw std::runtime_error("Wrong key: " + key);
            }
        }

        return MSBFSConfig(graph_path, starting_vertices);
    }
};