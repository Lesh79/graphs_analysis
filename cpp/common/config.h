#pragma once

#include <string>

std::string Trim(std::string const& str) {
    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

class Config {
protected:
    std::string graph_path_;

public:
    Config(std::string const& graph_path) : graph_path_(graph_path) {}

    std::string const& GetGraphPath() const {
        return graph_path_;
    }
};