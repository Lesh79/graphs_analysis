#include "algo.h"

#include <LAGraph.h>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <vector>

using clocks = std::chrono::steady_clock;

GBPageRank::GBPageRank(double alpha, double eps, int max_iter)
    : alpha_(alpha), eps_(eps), max_iter_(max_iter) {}

void GBPageRank::RunAlgo(GBGraph const& graph) {
    if (!graph.is_inited || graph.matrix == nullptr) {
        throw std::runtime_error("Graph not initialized");
    }

    GrB_Index nrows, ncols;
    GrB_Info status = GrB_Matrix_nrows(&nrows, graph.matrix);
    GrB_Matrix_ncols(&ncols, graph.matrix);
    if (nrows != ncols) {
        throw std::runtime_error("Graph must be square");
    }

    auto start = clocks::now();
    rank_ = ComputePageRankCore(graph, alpha_, eps_, max_iter_);
    auto end = clocks::now();
    exec_time_ = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    parsed_ = false;
}

GrB_Vector GBPageRank::ComputePageRankCore(GBGraph const& graph, double alpha, double eps,
                                           int max_iter) {
    GrB_Vector rank;
    GrB_Vector_new(&rank, GrB_FP64, graph.n_nodes);

    GrB_Matrix matrix = graph.matrix;

    LAGraph_Graph G;
    int status = LAGraph_New(&G, &matrix, LAGraph_ADJACENCY_DIRECTED, msg_);
    if (status != GrB_SUCCESS) {
        std::cout << "Failed to create LAGraph_Graph: " << status << std::endl;
        throw std::runtime_error(std::string("LAGraph_New failed: ") + msg_);
    }

    status = LAGraph_Cached_AT(G, msg_);
    if (status != GrB_SUCCESS) {
        LAGraph_Delete(&G, msg_);
        std::cout << "Failed to cache LAGraph_AT: " << msg_ << std::endl;
        throw std::runtime_error(std::string("LAGraph_Cached_AT failed: ") + msg_);
    }

    status = LAGraph_Cached_OutDegree(G, msg_);
    if (status != GrB_SUCCESS) {
        LAGraph_Delete(&G, msg_);
        std::cout << "Failed to cache LAGraph_OutDegree: " << msg_ << std::endl;
        throw std::runtime_error(std::string("LAGraph_Cached_OutDegree failed: ") + msg_);
    }

    int niters = 0;
    msg_[0] = '\0';
    status = LAGr_PageRank(&rank, &niters, G, alpha, eps, max_iter, msg_);
    if (status != GrB_SUCCESS) {
        LAGraph_Delete(&G, msg_);
        std::clog << "Failed to compute PageRank: " << status << std::endl;
        // throw std::runtime_error(std::string("LAGr_PageRank failed: ") + msg_);
    }

    LAGraph_Delete(&G, msg_);
    return rank;
}

void GBPageRank::ParseResult() {
    parsed_ = true;
    result_.clear();

    if (rank_ == nullptr) return;

    GrB_Index n;
    GrB_Vector_size(&n, rank_);
    result_.resize(n);

    std::vector<GrB_Index> indices(n);
    std::vector<double> values(n);
    GrB_Index nvals = n;
    GrB_Vector_extractTuples_FP64(indices.data(), values.data(), &nvals, rank_);

    for (GrB_Index i = 0; i < nvals; i++) {
        result_[indices[i]] = values[i];
    }
}

std::vector<double> const& GBPageRank::GetResult() const {
    if (!parsed_ && rank_ != nullptr) const_cast<GBPageRank*>(this)->ParseResult();
    return result_;
}

std::chrono::milliseconds GBPageRank::GetExecTime() const {
    return exec_time_;
}
