#pragma once

#include <spla.hpp>

struct SPLAGraph {
    int n_edges;
    int n_vertices;
    spla::ref_ptr<spla::Matrix> matrix;
};
