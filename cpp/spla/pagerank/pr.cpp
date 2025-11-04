#include "pr.h"

#include <chrono>

double PageRankRunner::RunAlgo(SPLAGraph const& graph) {
    auto start = std::chrono::high_resolution_clock::now();

    spla::ref_ptr<spla::Descriptor> desc = spla::Descriptor::make();
    desc->set_early_exit(true);
    desc->set_struct_only(true);

    spla::ref_ptr<spla::Vector> degrees = spla::Vector::make(graph.n_vertices, spla::INT);
    spla::exec_m_reduce_by_row(degrees, graph.matrix, spla::PLUS_INT, SPLA_ZERO_INT, desc);

    r = spla::Vector::make(graph.n_vertices, spla::FLOAT);

    spla::ref_ptr<spla::Matrix> A =
            spla::Matrix::make(graph.n_vertices, graph.n_vertices, spla::FLOAT);

    for (auto [src, dest] : indices_) {
        int degree;
        degrees->get_int(src, degree);
        A->set_float(src, dest, 1.0 / degree);
    }

    spla::pr(r, A, 0.85, 1e-4, desc);

    auto end = std::chrono::high_resolution_clock::now();

    auto s = spla::Scalar::make_float(0);

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

spla::ref_ptr<spla::Vector> const& PageRankRunner::GetResult() const {
    return r;
}