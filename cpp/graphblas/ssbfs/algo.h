#pragma once

#include <GraphBLAS.h>
#include <chrono>
#include <vector>

#include "algorunner.h"
#include "graph.h"

class GBSSBFS : public AlgoRunner<std::vector<GrB_Index>> {
public:
    explicit GBSSBFS(GrB_Index source);

    void RunAlgo(GBGraph const& graph) override;

    std::vector<GrB_Index> const& GetResult() const override;

    std::chrono::milliseconds GetExecTime() const;

private:
    GrB_Vector ComputeBFSCore(GrB_Matrix const& A, GrB_Index source);

    void ParseResult();

    GrB_Index source_;
    mutable GrB_Vector parent_{nullptr};
    mutable std::vector<GrB_Index> result_;
    std::chrono::milliseconds exec_time_{0};
    mutable bool parsed_{false};
};
