#include "algo.h"

#include <LAGraphX.h>
#include <chrono>
#include <iostream>
#include <stdexcept>

using clock_ = std::chrono::steady_clock;

GBMST::GBMST() = default;

void GBMST::RunAlgo(GBGraph const& graph) {
    if (!graph.is_inited || graph.matrix == nullptr) {
        throw std::runtime_error("Graph not initialized");
    }

    GrB_Index nrows, ncols;
    GrB_Matrix_nrows(&nrows, graph.matrix);
    GrB_Matrix_ncols(&ncols, graph.matrix);

    if (nrows != ncols) {
        throw std::runtime_error("Graph must be square");
    }

    auto start = clock_::now();
    ComputeMST(graph);
    auto end = clock_::now();
    exec_time_ = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    parsed_ = false;
}

void GBMST::ComputeMST(GBGraph const& graph) {
    int status = LAGraph_msf(&mst_matrix_, &component_vec_, graph.matrix, false, msg_);
    if (status != GrB_SUCCESS) {
        throw std::runtime_error(std::string("LAGraph_msf failed: ") + msg_);
    }

    uint64_t total_weight = 0;
    status = GrB_Matrix_reduce_UINT64(&total_weight, GrB_NULL, GrB_PLUS_MONOID_UINT64, mst_matrix_,
                                      GrB_NULL);
    if (status != GrB_SUCCESS) {
        throw std::runtime_error("Failed to reduce MST matrix");
    }

    result_.num_vertices = 0;
    GrB_Matrix_nrows(&result_.num_vertices, graph.matrix);
    result_.total_weight = total_weight;
}

void GBMST::ParseResult() {
    if (mst_matrix_ == nullptr || component_vec_ == nullptr) {
        throw std::runtime_error("MST matrix or component vector is null");
    }

    result_.parent.resize(result_.num_vertices, -1);

    for (GrB_Index i = 0; i < result_.num_vertices; ++i) {
        GrB_Index comp;
        if (GrB_Vector_extractElement_UINT64(&comp, component_vec_, i) == GrB_SUCCESS) {
            result_.parent[i] = static_cast<int>(comp);
        }
    }

    GrB_Matrix_free(&mst_matrix_);
    GrB_Vector_free(&component_vec_);
    parsed_ = true;
}

GBTree const& GBMST::GetResult() const {
    if (!parsed_ && mst_matrix_ != nullptr) const_cast<GBMST*>(this)->ParseResult();
    return result_;
}

std::chrono::milliseconds GBMST::GetExecTime() const {
    return exec_time_;
}
