#pragma once

#include <fstream>

#include "graph.h"

class Parser {
public:
    GBGraph ParseDIMACS(std::string const& filepath, bool weights = false);
    GBGraph ParseSNAP(std::string const& filepath);
};