#include "algo.h"

#include <stdexcept>
#include <vector>
#include <limits>
#include <chrono>
#include <cmath>

using clocks = std::chrono::steady_clock;

GBPageRank::GBPageRank(double alpha, double eps, int max_iter)
    : alpha_(alpha), eps_(eps), max_iter_(max_iter) {}


void GBPageRank::RunAlgo(const GBGraph& graph) {
    if (!graph.is_inited || graph.matrix == nullptr) {
        throw std::runtime_error("Graph not initialized");
    }

    GrB_Index nrows, ncols;
    GrB_Matrix_nrows(&nrows, graph.matrix);
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

GrB_Vector GBPageRank::ComputePageRankCore(const GBGraph& graph, double alpha, double eps, int max_iter) {
}

void GBPageRank::ParseResult() {
}

const std::vector<double>& GBPageRank::GetResult() const {
    if (!parsed_ && rank_ != nullptr) {
        const_cast<GBPageRank*>(this)->ParseResult();
    }
    return result_;
}

std::chrono::milliseconds GBPageRank::GetExecTime() const {
    return exec_time_;
}
