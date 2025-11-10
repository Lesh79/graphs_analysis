#pragma once

#include <GraphBLAS.h>
#include <LAGraph.h>
#include <chrono>
#include <vector>

#include "algorunner.h"
#include "graph.h"

struct GBTree {
    GrB_Index num_vertices;
    std::vector<int> parent;
    uint64_t total_weight;
};

class GBMST : public AlgoRunner<GBTree> {
public:
    GBMST();

    void RunAlgo(GBGraph const& graph) override;
    GBTree const& GetResult() const override;
    std::chrono::milliseconds GetExecTime() const;

private:
    void ComputeMST(GBGraph const& graph);
    void ParseResult();

    char msg_[LAGRAPH_MSG_LEN];
    GrB_Matrix mst_matrix_ = nullptr;
    GrB_Vector component_vec_ = nullptr;
    GBTree result_;
    std::chrono::milliseconds exec_time_;
    bool parsed_ = false;
};
