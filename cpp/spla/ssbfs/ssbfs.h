#pragma once

#include <spla.hpp>
#include <vector>

#include "algorunner.h"

class SSBFSRunner : public AlgoRunner<spla::ref_ptr<spla::Vector>> {
private:
    spla::ref_ptr<spla::Vector> parent_;
    int s_;

public:
    SSBFSRunner(int s) : s_(s) {}

    virtual double RunAlgo(SPLAGraph const& graph);
    virtual spla::ref_ptr<spla::Vector> const& GetResult() const;
};