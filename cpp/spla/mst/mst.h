#pragma once
#include <spla.hpp>

#include "algorunner.h"

class MSTRunner : public AlgoRunner<double> {
private:
    spla::ref_ptr<spla::Matrix> spanning_tree_;
    int weight_;

public:
    MSTRunner() : weight_(0) {};

    virtual int RunAlgo(SPLAGraph const& graph);
    virtual double const& GetResult() const;

    spla::ref_ptr<spla::Matrix> GetSpanningTree() const {
        return spanning_tree_;
    }
};