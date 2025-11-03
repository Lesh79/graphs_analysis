#include "algo.h"

#include <stdexcept>
#include <limits>
#include <vector>
#include <chrono>

using clocks = std::chrono::steady_clock;

GBSSBFS::GBSSBFS(GrB_Index source) : source_(source) {}

void GBSSBFS::RunAlgo(const GBGraph &graph) {
    if (!graph.is_inited || graph.matrix == nullptr) {
        throw std::runtime_error("Graph not initialized");
    }

    GrB_Index nrows, ncols;
    GrB_Matrix_nrows(&nrows, graph.matrix);
    GrB_Matrix_ncols(&ncols, graph.matrix);
    if (nrows != ncols) {
        throw std::runtime_error("Graph matrix must be square");
    }

    auto start = clocks::now();
    parent_ = ComputeBFSCore(graph.matrix, source_);
    auto end = clocks::now();

    exec_time_ = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    parsed_ = false; 
}

GrB_Vector GBSSBFS::ComputeBFSCore(const GrB_Matrix &A, GrB_Index source) {
    GrB_Index nrows;
    GrB_Matrix_nrows(&nrows, A);

    GrB_Vector front, visited, parent, next_parents;
    GrB_Vector_new(&front, GrB_BOOL, nrows);
    GrB_Vector_new(&visited, GrB_BOOL, nrows);
    GrB_Vector_new(&parent, GrB_INT64, nrows);
    GrB_Vector_new(&next_parents, GrB_INT64, nrows);

    GrB_Vector_setElement_BOOL(front, true, source);
    GrB_Vector_setElement_BOOL(visited, true, source);
    GrB_Vector_setElement_INT64(parent, source, source);

    while (true) {
        GrB_vxm(next_parents, GrB_NULL, GrB_NULL,
                GxB_ANY_SECONDI_INT64, front, A, GrB_NULL);

        GrB_Vector not_visited;
        GrB_Vector_new(&not_visited, GrB_BOOL, nrows);
        GrB_Vector_assign_BOOL(not_visited, GrB_NULL, GrB_NULL, true, GrB_ALL, nrows, GrB_NULL);
        GrB_Vector_assign_BOOL(not_visited, visited, GrB_NULL, false, GrB_ALL, nrows, GrB_DESC_S);

        GrB_Vector next_filtered;
        GrB_Vector_new(&next_filtered, GrB_INT64, nrows);
        GrB_Vector_eWiseMult_BinaryOp(next_filtered, not_visited, GrB_NULL,
                                      GrB_SECOND_INT64, next_parents, next_parents, GrB_NULL);

        GrB_Index nvals;
        GrB_Vector_nvals(&nvals, next_filtered);
        if (nvals == 0) {
            GrB_Vector_free(&not_visited);
            GrB_Vector_free(&next_filtered);
            break;
        }

        GrB_Vector_assign(parent, next_filtered, GrB_NULL, next_filtered, GrB_ALL, nrows, GrB_DESC_S);

        GrB_Vector_free(&front);
        GrB_Vector_new(&front, GrB_BOOL, nrows);
        GrB_Vector_assign_BOOL(front, next_filtered, GrB_NULL, true, GrB_ALL, nrows, GrB_DESC_S);

        GrB_Vector_eWiseAdd_BinaryOp(visited, GrB_NULL, GrB_NULL,
                                     GrB_LOR, visited, front, GrB_NULL);

        GrB_Vector_free(&not_visited);
        GrB_Vector_free(&next_filtered);
    }

    GrB_Vector_free(&front);
    GrB_Vector_free(&visited);
    GrB_Vector_free(&next_parents);

    return parent;
}

void GBSSBFS::ParseResult() {
    if (parent_ == nullptr) {
        throw std::runtime_error("No BFS data available. RunAlgo must be called first.");
    }
    if (parsed_) return;

    GrB_Index nrows;
    GrB_Vector_size(&nrows, parent_);

    result_.assign(nrows, std::numeric_limits<GrB_Index>::max());

    GrB_Index nvals;
    GrB_Vector_nvals(&nvals, parent_);

    std::vector<GrB_Index> indices(nvals);
    std::vector<int64_t> values(nvals);
    GrB_Vector_extractTuples_INT64(indices.data(), values.data(), &nvals, parent_);

    for (GrB_Index i = 0; i < nvals; i++) {
        result_[indices[i]] = values[i];
    }

    parsed_ = true;
}

const std::vector<GrB_Index> &GBSSBFS::GetResult() const {
    if (!parsed_ && parent_ != nullptr) {
        const_cast<GBSSBFS *>(this)->ParseResult();
    }
    return result_;
}

std::chrono::milliseconds GBSSBFS::GetExecTime() const {
    return exec_time_;
}
