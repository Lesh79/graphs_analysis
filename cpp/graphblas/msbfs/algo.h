#pragma once

#include "algorunner.h"
#include "graph.h"

#include <GraphBLAS.h>
#include <vector>

class GBMSBFS : public AlgoRunner<std::vector<std::vector<GrB_Index>>> {
public:
    explicit GBMSBFS(const std::vector<GrB_Index>& sources);

    void RunAlgo(GBGraph const& graph) override;

    const std::vector<std::vector<GrB_Index>>& GetResult() const override;

private:
    std::vector<GrB_Index> sources_;
    std::vector<std::vector<GrB_Index>> result_;
};
