#pragma once

#include <fstream>
#include <sstream>

#include "config.h"

class PRConfig : public Config {
private:
    double damp_;
    double tol_;

public:
    PRConfig(std::string graph_path, double damp = 0.85, double tol = 0.001)
        : Config(graph_path), damp_(damp), tol_(tol) {}

    double GetDampingFactor() const {
        return damp_;
    }

    double GetTolerance() const {
        return tol_;
    }

    static PRConfig Parse(std::string config_path) {
        std::ifstream fin(config_path);
        if (!fin.is_open()) {
            throw std::runtime_error("Failed to open file " + config_path);
        }
        std::string line;

        std::string graph_path;
        double damp;
        double tol;

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
            } else if (key == "Damping") {
                damp = std::stod(value);
            } else if (key == "Tolerance") {
                tol = std::stod(value);
            } else {
                throw std::runtime_error("Wrong key: " + key);
            }
        }
        return PRConfig(graph_path, damp, tol);
    }
};