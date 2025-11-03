#pragma once

#include "algorunner.h"
#include "graph.h"

#include <GraphBLAS.h>
#include <vector>

class GBSSBFS : public AlgoRunner<std::vector<GrB_Index>> {
public:
    explicit GBSSBFS(GrB_Index source);

    void RunAlgo(GBGraph const& graph) override;

    const std::vector<GrB_Index>& GetResult() const override;

private:
    GrB_Index source_;
    std::vector<GrB_Index> result_;
};
