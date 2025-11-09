#pragma once
#include <spla.hpp>

#include "graph.h"

template <typename ResultT>
class AlgoRunner {
public:
    virtual int RunAlgo(SPLAGraph const& graph) = 0;
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

    void ReduceByVectors(spla::ref_ptr<spla::Scalar>& r, spla::ref_ptr<spla::Matrix>& m,
                         spla::ref_ptr<spla::OpBinary>& op, spla::ref_ptr<spla::Descriptor>& desc) {
        spla::ref_ptr<spla::Vector> v = spla::Vector::make(m->get_n_cols(), spla::INT);

        spla::exec_m_reduce_by_row(v, m, op, SPLA_ZERO_INT, desc);
        spla::exec_v_reduce(r, SPLA_ZERO_INT, v, op, desc);
    }
};