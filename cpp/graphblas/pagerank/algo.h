#pragma once

#include "algorunner.h"
#include "graph.h"

#include <GraphBLAS.h>
#include <LAGraph.h>
#include <vector>
#include <chrono>

class GBPageRank : public AlgoRunner<std::vector<double>> {
public:
    explicit GBPageRank(double alpha = 0.85, double eps = 1e-6, int max_iter = 10000);

    void RunAlgo(const GBGraph& graph) override;

    const std::vector<double>& GetResult() const override;

    std::chrono::milliseconds GetExecTime() const;

private:
    GrB_Vector ComputePageRankCore(const GBGraph& graph, double alpha, double eps, int max_iter);
    void ParseResult();

    double alpha_;
    double eps_;
    int max_iter_;

    char msg_[LAGRAPH_MSG_LEN];
    GrB_Vector rank_ = nullptr;
    std::vector<double> result_;
    std::chrono::milliseconds exec_time_;
    bool parsed_ = false;
};
