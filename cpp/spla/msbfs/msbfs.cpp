#include "msbfs.h"

#include <chrono>
#include <limits.h>

void print_m(spla::ref_ptr<spla::Matrix> const& m, std::string const& name) {
    std::cout << "-- " << name << " --\n";
    for (spla::uint i = 0; i < m->get_n_rows(); ++i) {
        for (spla::uint j = 0; j < m->get_n_cols(); ++j) {
            int32_t x;
            m->get_int(i, j, x);
            std::cout << x << ' ';
        }
        std::cout << std::endl;
    }
}

void MSBFSRunner::ApplyMaskOnFront(spla::ref_ptr<spla::Matrix>& front,
                                   spla::ref_ptr<spla::Matrix>& mask,
                                   spla::ref_ptr<spla::Descriptor>& desc) {
    spla::ref_ptr<spla::Scalar> nnz_mask_count = spla::Scalar::make_int(0);
    ReduceByVectors(nnz_mask_count, mask, spla::PLUS_INT, desc);
    std::vector<spla::T_INT> bf1, bf2, bf3;

    spla::ref_ptr<spla::MemView> keys1_view =
            spla::MemView::make(bf1.data(), nnz_mask_count->as_int());
    spla::ref_ptr<spla::MemView> keys2_view =
            spla::MemView::make(bf2.data(), nnz_mask_count->as_int());
    spla::ref_ptr<spla::MemView> values_view =
            spla::MemView::make(bf3.data(), nnz_mask_count->as_int());

    mask->read(keys1_view, keys2_view, values_view);

    spla::T_INT* keys1 = static_cast<spla::T_INT*>(keys1_view->get_buffer());
    spla::T_INT* keys2 = static_cast<spla::T_INT*>(keys2_view->get_buffer());
    spla::T_INT* values = static_cast<spla::T_INT*>(values_view->get_buffer());

    for (spla::T_INT i = 0; i < nnz_mask_count->as_int(); ++i) {
        if (values[i] != INT_MAX) {
            front->set_int(keys1[i], keys2[i], 0);
        }
    }
}

void MSBFSRunner::ReduceByVectors(spla::ref_ptr<spla::Scalar>& r, spla::ref_ptr<spla::Matrix>& m,
                                  spla::ref_ptr<spla::OpBinary>& op,
                                  spla::ref_ptr<spla::Descriptor>& desc) {
    spla::ref_ptr<spla::Vector> v = spla::Vector::make(m->get_n_cols(), spla::INT);
    spla::ref_ptr<spla::Scalar> tmp = spla::Scalar::make_int(0);

    r->set_int(0);

    for (spla::uint i = 0; i < m->get_n_rows(); ++i) {
        spla::exec_m_extract_row(v, m, i, spla::IDENTITY_INT, desc);
        spla::exec_v_reduce(tmp, SPLA_ZERO_INT, v, op, desc);

        r->set_int(r->as_int() + tmp->as_int());
    }
}

double MSBFSRunner::RunAlgo(SPLAGraph const& graph) {
    auto start = std::chrono::high_resolution_clock::now();

    spla::ref_ptr<spla::Descriptor> desc = spla::Descriptor::make();
    desc->set_early_exit(true);
    desc->set_struct_only(true);

    spla::ref_ptr<spla::Matrix> prev_fronts =
            spla::Matrix::make(ss_.size(), graph.n_vertices, spla::INT);
    prev_fronts->set_fill_value(SPLA_ZERO_INT);

    spla::ref_ptr<spla::Scalar> frontier_size = spla::Scalar::make_int(ss_.size());

    bool fronts_empty = false;

    parents_ = spla::Matrix::make(ss_.size(), graph.n_vertices, spla::INT);
    parents_->set_fill_value(SPLA_ZERO_INT);

    spla::ref_ptr<spla::Matrix> p = spla::Matrix::make(ss_.size(), graph.n_vertices, spla::INT);
    p->set_fill_value(SPLA_ZERO_INT);

    spla::ref_ptr<spla::Matrix> p_updated =
            spla::Matrix::make(ss_.size(), graph.n_vertices, spla::INT);
    p_updated->set_fill_value(SPLA_ZERO_INT);

    spla::ref_ptr<spla::Matrix> p_mask =
            spla::Matrix::make(ss_.size(), graph.n_vertices, spla::INT);
    p_mask->set_fill_value(SPLA_ZERO_INT);

    int i = 0;
    for (auto s : ss_) {
        prev_fronts->set_int(i, s, s + 1);
        parents_->set_int(i, s, s + 1);
        ++i;
    }

    while (!fronts_empty) {
        spla::exec_mxm(p, prev_fronts, graph.matrix, spla::FIRST_INT, spla::MIN_NON_ZERO_INT,
                       SPLA_ZERO_INT, desc);

        spla::exec_m_emult(p_mask, p, parents_, spla::BONE_INT, desc);

        spla::exec_m_eadd(p_updated, parents_, p, spla::FIRST_INT, desc);

        ApplyMaskOnFront(p, p_mask, desc);

        ReduceByVectors(frontier_size, p, spla::PLUS_INT, desc);

        std::swap(p, prev_fronts);
        std::swap(p_updated, parents_);

        fronts_empty = frontier_size->as_int() <= 0;
    }

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

spla::ref_ptr<spla::Matrix> const& MSBFSRunner::GetResult() const {
    return parents_;
}
