#include "pr.h"

#include <chrono>
#include <cmath>

double PageRankRunner::RunAlgo(SPLAGraph const& graph) {
    int N = graph.n_vertices;

    spla::ref_ptr<spla::Descriptor> desc = spla::Descriptor::make();
    desc->set_early_exit(true);
    desc->set_struct_only(true);

    spla::ref_ptr<spla::Vector> degrees = spla::Vector::make(N, spla::INT);
    spla::exec_m_reduce_by_row(degrees, graph.matrix, spla::PLUS_INT, SPLA_ZERO_INT, desc);

    r = spla::Vector::make(N, spla::FLOAT);

    spla::ref_ptr<spla::Matrix> A = spla::Matrix::make(N, N, spla::FLOAT);

    for (auto [src, dest] : indices_) {
        int degree;
        degrees->get_int(src, degree);
        A->set_float(src, dest, 1.0 / degree);
    }

    auto start = std::chrono::high_resolution_clock::now();

    spla::ref_ptr<spla::Vector> full_mask = spla::Vector::make(N, spla::FLOAT);
    spla::ref_ptr<spla::Vector> r_prev = spla::Vector::make(N, spla::FLOAT);
    spla::ref_ptr<spla::Vector> r_tmp = spla::Vector::make(N, spla::FLOAT);
    spla::ref_ptr<spla::Vector> addition = spla::Vector::make(N, spla::FLOAT);
    spla::ref_ptr<spla::Vector> errors = spla::Vector::make(N, spla::FLOAT);
    spla::ref_ptr<spla::Scalar> error2 = spla::Scalar::make(spla::FLOAT);

    addition->fill_with(spla::Scalar::make_float((1.0f - damp_) / static_cast<float>(N)));
    r_prev->fill_with(spla::Scalar::make_float(1.0f / static_cast<float>(N)));

    float error = tol_ + 0.1f;

    int iter = 0;
    while (iter++ <= max_iter_ && error > tol_) {
        spla::exec_mxv_masked(r_tmp, full_mask, A, r_prev, spla::MULT_FLOAT, spla::PLUS_FLOAT,
                              spla::ALWAYS_FLOAT, SPLA_ZERO_FLOAT);
        spla::exec_v_eadd(r, r_tmp, addition, spla::PLUS_FLOAT);

        exec_v_eadd(errors, r, r_prev, spla::MINUS_POW2_FLOAT);
        exec_v_reduce(error2, SPLA_ZERO_FLOAT, errors, spla::PLUS_FLOAT);

        error = std::sqrt(error2->as_float());

        std::swap(r, r_prev);
    }

    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

spla::ref_ptr<spla::Vector> const& PageRankRunner::GetResult() const {
    return r;
}