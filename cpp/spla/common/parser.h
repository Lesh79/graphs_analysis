#pragma once

#include <fstream>
#include <spla.hpp>

#include "graph.h"

class Parser {
public:
    SPLAGraph ParseDIMACS(std::string const& filepath, bool weights = true);
};