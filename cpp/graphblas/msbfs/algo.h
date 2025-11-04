#pragma once

#include <GraphBLAS.h>
#include <chrono>
#include <vector>

#include "algorunner.h"
#include "graph.h"

class GBMSBFS : public AlgoRunner<std::vector<std::vector<GrB_Index>>> {
public:
    explicit GBMSBFS(std::vector<int> const& sources);

    void RunAlgo(GBGraph const& graph) override;

    std::vector<std::vector<GrB_Index>> const& GetResult() const override;

    std::chrono::milliseconds GetExecTime() const;

private:
    static GrB_Matrix ComputeMSBFScore(GBGraph const& graph, std::vector<int> const& sources);

    void ParseResult();

    std::vector<int> sources_;
    mutable GrB_Matrix parent_{nullptr};
    mutable std::vector<std::vector<GrB_Index>> result_;
    std::chrono::milliseconds exec_time_{0};
    mutable bool parsed_{false};
};
