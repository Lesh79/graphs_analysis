#pragma once

#include "algorunner.h"

class PageRankRunner : public AlgoRunner<spla::ref_ptr<spla::Vector>> {
private:
    spla::ref_ptr<spla::Vector> r;
    std::vector<std::pair<int, int>> indices_;

    double damp_;
    double tol_;

public:
    PageRankRunner(std::vector<std::pair<int, int>> const& indices, double damp = 0.85,
                   double tol = 1e-4)
        : indices_(indices), damp_(damp), tol_(tol) {}

    virtual double RunAlgo(SPLAGraph const& graph);
    virtual spla::ref_ptr<spla::Vector> const& GetResult() const;
};