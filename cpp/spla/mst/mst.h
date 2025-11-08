#pragma once
#include <spla.hpp>

#include "algorunner.h"

class MSTRunner : public AlgoRunner<double> {
private:
    spla::ref_ptr<spla::Vector> parent_;
    double weight_ = 0;

public:
    MSTRunner() = default;

    virtual double RunAlgo(SPLAGraph const& graph);
    virtual double const& GetResult() const;
};