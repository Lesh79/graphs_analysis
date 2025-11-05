#pragma once
#include <spla.hpp>

#include "algorunner.h"

class MSTRunner : public AlgoRunner<double> {
private:
    spla::ref_ptr<spla::Vector> parent_;
    double weight_ = 0;

    void ReduceByVectors(spla::ref_ptr<spla::Scalar>& r, spla::ref_ptr<spla::Matrix>& m,
                         spla::ref_ptr<spla::OpBinary>& op, spla::ref_ptr<spla::Descriptor>& desc);

public:
    MSTRunner() = default;

    virtual double RunAlgo(SPLAGraph const& graph);
    virtual double const& GetResult() const;
};