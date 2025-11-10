#pragma once

#include <fstream>
#include <sstream>

#include "config.h"

class MSTConfig : public Config {
public:
    MSTConfig(std::string graph_path) : Config(graph_path) {}

    static MSTConfig Parse(std::string config_path) {
        std::ifstream fin(config_path);
        if (!fin.is_open()) {
            throw std::runtime_error("Failed to open file " + config_path);
        }
        std::string line;

        std::string graph_path;

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
            } else {
                throw std::runtime_error("Wrong key: " + key);
            }
        }
        return MSTConfig(graph_path);
    }
};