#pragma once

#include <GraphBLAS.h>

struct GBGraph {
    GrB_Index n_nodes;
    GrB_Index n_arcs;
    GrB_Matrix matrix;
    bool is_inited;
};