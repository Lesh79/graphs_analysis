#pragma once

#include "algorunner.h"
#include "graph.h"

#include <GraphBLAS.h>
#include <vector>
#include <chrono>

class GBMSBFS : public AlgoRunner<std::vector<std::vector<GrB_Index>>> {
public:
    explicit GBMSBFS(const std::vector<GrB_Index>& sources);

    void RunAlgo(const GBGraph& graph) override;

    const std::vector<std::vector<GrB_Index>>& GetResult() const override;

    std::chrono::milliseconds GetExecTime() const;

private:
    static GrB_Matrix ComputeMSBFScore(const GBGraph& graph, const std::vector<GrB_Index>& sources);

    void ParseResult();

    std::vector<GrB_Index> sources_;
    mutable GrB_Matrix parent_{nullptr};
    mutable std::vector<std::vector<GrB_Index>> result_;
    std::chrono::milliseconds exec_time_{0};
    mutable bool parsed_{false};
};
