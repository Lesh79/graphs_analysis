#pragma once

#include "algo.h"
#include "algorunner.h"
#include "graph.h"

#include <GraphBLAS.h>
#include <stdexcept>
#include <vector>

GBSSBFS::GBSSBFS(GrB_Index source) : source_(source) {}

void GBSSBFS::RunAlgo(GBGraph const& graph) {
    if (!graph.is_inited || graph.matrix == nullptr) {
        throw std::runtime_error("Graph not initialized");
    }

    GrB_Index nrows, ncols;
    GrB_Matrix_nrows(&nrows, graph.matrix);
    GrB_Matrix_ncols(&ncols, graph.matrix);
    if (nrows != ncols) {
        throw std::runtime_error("Graph matrix must be square");
    }

    GrB_Info info;

    GrB_Vector front, visited, parent;
    info = GrB_Vector_new(&front, GrB_BOOL, nrows);
    info = GrB_Vector_new(&visited, GrB_BOOL, nrows);
    info = GrB_Vector_new(&parent, GrB_INT64, nrows);

    GrB_Vector_setElement_BOOL(front, true, source_);
    GrB_Vector_setElement_BOOL(visited, true, source_);
    GrB_Vector_setElement_INT64(parent, source_, source_);

    GrB_Vector next_parents;
    GrB_Vector_new(&next_parents, GrB_INT64, nrows);

    while (true) {
        GrB_vxm(next_parents, GrB_NULL, GrB_NULL,
                GxB_ANY_SECONDI_INT64, front, graph.matrix, GrB_NULL);

        GrB_Vector not_visited;
        GrB_Vector_new(&not_visited, GrB_BOOL, nrows);
        GrB_Vector_assign_BOOL(not_visited, GrB_NULL, GrB_NULL, true, GrB_ALL, nrows, GrB_NULL);
        GrB_Vector_assign_BOOL(not_visited, visited, GrB_NULL, false, GrB_ALL, nrows, GrB_DESC_S);

        GrB_Vector next_filtered;
        GrB_Vector_new(&next_filtered, GrB_INT64, nrows);
        GrB_Vector_eWiseMult_BinaryOp(next_filtered, not_visited, GrB_NULL, GrB_SECOND_INT64, next_parents, next_parents, GrB_NULL);

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

        GrB_Vector_eWiseAdd_BinaryOp(
            visited, GrB_NULL, GrB_NULL, GrB_LOR,
            visited, front, GrB_NULL
        );

        GrB_Vector_free(&not_visited);
        GrB_Vector_free(&next_filtered);
    }

    result_.clear();
    result_.resize(nrows, std::numeric_limits<GrB_Index>::max());

    GrB_Index nvals_parent;
    GrB_Vector_nvals(&nvals_parent, parent);

    std::vector<GrB_Index> indices(nvals_parent);
    std::vector<int64_t> values(nvals_parent);

    GrB_Vector_extractTuples_INT64(indices.data(), values.data(), &nvals_parent, parent);

    for (GrB_Index i = 0; i < nvals_parent; i++) {
        result_[indices[i]] = values[i];
    }

    GrB_Vector_free(&front);
    GrB_Vector_free(&visited);
    GrB_Vector_free(&parent);
    GrB_Vector_free(&next_parents);
}

const std::vector<GrB_Index>& GBSSBFS::GetResult() const {
    return result_;
}
