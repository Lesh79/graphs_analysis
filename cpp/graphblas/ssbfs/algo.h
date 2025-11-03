#pragma once

#include "algorunner.h"
#include "graph.h"

#include <GraphBLAS.h>
#include <vector>
#include <chrono>

class GBSSBFS : public AlgoRunner<std::vector<GrB_Index>> {
public:
    explicit GBSSBFS(GrB_Index source);

    void RunAlgo(const GBGraph &graph) override;

    const std::vector<GrB_Index> &GetResult() const override;

    std::chrono::milliseconds GetExecTime() const;

private:
    static GrB_Vector ComputeBFSCore(const GrB_Matrix &A, GrB_Index source);

    void ParseResult();

    GrB_Index source_;
    mutable GrB_Vector parent_{nullptr};
    mutable std::vector<GrB_Index> result_;
    std::chrono::milliseconds exec_time_{0};
    mutable bool parsed_{false};
};
