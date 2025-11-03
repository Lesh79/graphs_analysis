#include "algo.h"
#include "graph.h"

#include <GraphBLAS.h>
#include <stdexcept>
#include <vector>
#include <limits>

GBMSBFS::GBMSBFS(const std::vector<GrB_Index>& sources) : sources_(sources) {}

void GBMSBFS::RunAlgo(GBGraph const& graph) {
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
    GrB_Index nsources = sources_.size();

    GrB_Matrix front, visited, parent;
    info = GrB_Matrix_new(&front, GrB_BOOL, nsources, nrows);
    info = GrB_Matrix_new(&visited, GrB_BOOL, nsources, nrows);
    info = GrB_Matrix_new(&parent, GrB_INT64, nsources, nrows);

    for (GrB_Index i = 0; i < nsources; i++) {
        GrB_Matrix_setElement_BOOL(front, true, i, sources_[i]);
        GrB_Matrix_setElement_BOOL(visited, true, i, sources_[i]);
        GrB_Matrix_setElement_INT64(parent, sources_[i], i, sources_[i]);
    }

    GrB_Matrix next_parents;
    GrB_Matrix_new(&next_parents, GrB_INT64, nsources, nrows);

    while (true) {
        GrB_mxm(next_parents, GrB_NULL, GrB_NULL, GxB_ANY_SECONDI_INT64, front, graph.matrix, GrB_NULL);

        GrB_Matrix not_visited;
        GrB_Matrix_new(&not_visited, GrB_BOOL, nsources, nrows);
        GrB_Matrix_assign_BOOL(not_visited, GrB_NULL, GrB_NULL, true, GrB_ALL, nsources, GrB_ALL, nrows, GrB_NULL);
        GrB_Matrix_assign_BOOL(not_visited, visited, GrB_NULL, false, GrB_ALL, nsources, GrB_ALL, nrows, GrB_DESC_S);

        GrB_Matrix next_filtered;
        GrB_Matrix_new(&next_filtered, GrB_INT64, nsources, nrows);
        GrB_Matrix_eWiseMult_BinaryOp(
            next_filtered, not_visited, GrB_NULL, GrB_SECOND_INT64,
            next_parents, next_parents, GrB_NULL
        );

        GrB_Index nvals;
        GrB_Matrix_nvals(&nvals, next_filtered);
        if (nvals == 0) {
            GrB_Matrix_free(&not_visited);
            GrB_Matrix_free(&next_filtered);
            break;
        }

        GrB_Matrix_assign(parent, next_filtered, GrB_NULL, next_filtered,
                          GrB_ALL, nsources, GrB_ALL, nrows, GrB_DESC_S);

        GrB_Matrix_free(&front);
        GrB_Matrix_new(&front, GrB_BOOL, nsources, nrows);
        GrB_Matrix_assign_BOOL(front, next_filtered, GrB_NULL, true,
                               GrB_ALL, nsources, GrB_ALL, nrows, GrB_DESC_S);

        GrB_Matrix_eWiseAdd_BinaryOp(
            visited, GrB_NULL, GrB_NULL, GrB_LOR,
            visited, front, GrB_NULL
        );

        GrB_Matrix_free(&not_visited);
        GrB_Matrix_free(&next_filtered);
    }

    result_.clear();
    result_.resize(nsources, std::vector<GrB_Index>(nrows, std::numeric_limits<GrB_Index>::max()));

    GrB_Index nvals_parent;
    GrB_Matrix_nvals(&nvals_parent, parent);

    std::vector<GrB_Index> I(nvals_parent), J(nvals_parent);
    std::vector<int64_t> X(nvals_parent);

    GrB_Matrix_extractTuples_INT64(I.data(), J.data(), X.data(), &nvals_parent, parent);

    for (GrB_Index idx = 0; idx < nvals_parent; idx++) {
        result_[I[idx]][J[idx]] = X[idx];
    }

    GrB_Matrix_free(&front);
    GrB_Matrix_free(&visited);
    GrB_Matrix_free(&parent);
    GrB_Matrix_free(&next_parents);
}

const std::vector<std::vector<GrB_Index>>& GBMSBFS::GetResult() const {
    return result_;
}
