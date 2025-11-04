#pragma once

#include <spla.hpp>
#include <vector>

#include "algorunner.h"

class MSBFSRunner : public AlgoRunner<spla::ref_ptr<spla::Matrix>> {
private:
    spla::ref_ptr<spla::Matrix> parents_;
    std::vector<int> ss_;

    void ApplyMaskOnFront(spla::ref_ptr<spla::Matrix>& front, spla::ref_ptr<spla::Matrix>& mask,
                          spla::ref_ptr<spla::Descriptor>& desc);
    void ReduceByVectors(spla::ref_ptr<spla::Scalar>& r, spla::ref_ptr<spla::Matrix>& m,
                         spla::ref_ptr<spla::OpBinary>& op, spla::ref_ptr<spla::Descriptor>& desc);

public:
    MSBFSRunner(std::vector<int> const& ss) : ss_(ss) {}

    virtual double RunAlgo(SPLAGraph const& graph);
    virtual spla::ref_ptr<spla::Matrix> const& GetResult() const;
};