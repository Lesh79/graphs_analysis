#pragma once
#include "graph.h"

template <typename ResultT>
class AlgoRunner {
public:
    virtual double RunAlgo(SPLAGraph const& graph) = 0;
    virtual ResultT const& GetResult() const = 0;

    AlgoRunner() = default;
    ~AlgoRunner() = default;

protected:
    spla::ref_ptr<spla::Scalar> SPLA_ZERO_INT = spla::Scalar::make_int(0);
    spla::ref_ptr<spla::Scalar> SPLA_ONE_INT = spla::Scalar::make_int(1);

    spla::ref_ptr<spla::Scalar> SPLA_ZERO_FLOAT = spla::Scalar::make_float(0);
    spla::ref_ptr<spla::Scalar> SPLA_ONE_FLOAT = spla::Scalar::make_float(1);

    spla::ref_ptr<spla::Scalar> SPLA_ZERO_UINT = spla::Scalar::make_uint(0);
    spla::ref_ptr<spla::Scalar> SPLA_ONE_UINT = spla::Scalar::make_uint(1);
};