#include "ssbfs.h"

int SSBFSRunner::RunAlgo(SPLAGraph const& graph) {
    spla::ref_ptr<spla::Descriptor> desc = spla::Descriptor::make();
    desc->set_early_exit(true);
    desc->set_struct_only(true);

    spla::ref_ptr<spla::Vector> prev_front = spla::Vector::make(graph.n_vertices, spla::INT);
    prev_front->set_fill_value(SPLA_ZERO_INT);

    spla::ref_ptr<spla::Scalar> frontier_size = spla::Scalar::make_int(1);

    bool front_empty = false;

    parent_ = spla::Vector::make(graph.n_vertices, spla::INT);
    parent_->set_fill_value(SPLA_ZERO_INT);

    spla::ref_ptr<spla::Vector> p = spla::Vector::make(graph.n_vertices, spla::INT);
    p->set_fill_value(SPLA_ZERO_INT);

    spla::ref_ptr<spla::Vector> p_updated = spla::Vector::make(graph.n_vertices, spla::INT);
    p_updated->set_fill_value(SPLA_ZERO_INT);

    prev_front->set_int(s_, s_ + 1);
    parent_->set_int(s_, s_ + 1);

    auto start = std::chrono::high_resolution_clock::now();

    while (!front_empty) {
        spla::exec_vxm_masked(p, parent_, prev_front, graph.matrix, spla::FIRST_INT,
                              spla::MIN_NON_ZERO_INT, spla::EQZERO_INT, SPLA_ZERO_INT, desc);

        spla::exec_v_eadd(p_updated, p, parent_, spla::PLUS_INT, desc);

        std::swap(parent_, p_updated);
        std::swap(p, prev_front);

        spla::exec_v_count_mf(frontier_size, prev_front, desc);
        front_empty = frontier_size->as_int() <= 0;
    }

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

spla::ref_ptr<spla::Vector> const& SSBFSRunner::GetResult() const {
    return parent_;
}