#pragma once

#include "algorunner.h"

class PageRankRunner : public AlgoRunner<spla::ref_ptr<spla::Vector>> {
private:
    spla::ref_ptr<spla::Vector> r;
    std::vector<std::pair<int, int>> indices_;

public:
    virtual double RunAlgo(SPLAGraph const& graph);
    virtual spla::ref_ptr<spla::Vector> const& GetResult() const;

    void SetIndices(std::vector<std::pair<int, int>> const& indices) {
        indices_ = indices;
    }
};