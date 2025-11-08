#include "mst.h"

#include <chrono>

double MSTRunner::RunAlgo(SPLAGraph const& graph) {
    int N = graph.n_vertices;

    spla::ref_ptr<spla::Descriptor> desc = spla::Descriptor::make();
    desc->set_early_exit(true);
    desc->set_struct_only(true);

    spla::ref_ptr<spla::Vector> edge = spla::Vector::make(N, spla::INT);
    parent_ = spla::Vector::make(N, spla::INT);
    for (int i = 0; i < N; ++i) {
        parent_->set_int(i, i + 1);
    }
    spla::ref_ptr<spla::Matrix> tmp = spla::Matrix::make(N, N, spla::INT);

    spla::ref_ptr<spla::Matrix> S = spla::Matrix::make(N, N, spla::INT);
    spla::exec_m_eadd(S, graph.matrix, tmp, spla::PLUS_INT, desc);
    spla::ref_ptr<spla::Matrix> S_new = spla::Matrix::make(N, N, spla::INT);

    spla::ref_ptr<spla::Vector> full_mask = spla::Vector::make(N, spla::INT);
    full_mask->fill_with(SPLA_ONE_INT);

    spla::ref_ptr<spla::Scalar> S_size = spla::Scalar::make_int(1);
    ReduceByVectors(S_size, S, spla::PLUS_INT, desc);

    bool S_empty = false;

    spla::ref_ptr<spla::Vector> cedge = spla::Vector::make(N, spla::INT);

    spla::ref_ptr<spla::Vector> t = spla::Vector::make(N, spla::INT);
    spla::ref_ptr<spla::Vector> mask = spla::Vector::make(N, spla::INT);
    spla::ref_ptr<spla::Vector> index = spla::Vector::make(N, spla::INT);
    spla::ref_ptr<spla::Vector> parent_updated = spla::Vector::make(N, spla::INT);

    auto start = std::chrono::high_resolution_clock::now();

    while (!S_empty) {
        mask->clear();
        t->clear();
        index->clear();
        S_new->clear();

        spla::exec_mxv_masked(edge, full_mask, S, parent_, spla::SECOND_INT, spla::MAX_INT,
                              spla::ALWAYS_INT, SPLA_ZERO_INT, desc);

        for (int i = 0; i < N; ++i) {
            int parent_index, edge_v, cedge_v;
            parent_->get_int(i, parent_index);
            edge->get_int(i, edge_v);
            cedge->get_int(parent_index - 1, cedge_v);
            if (cedge_v == 0) {
                cedge->set_int(parent_index - 1, edge_v);
            } else {
                cedge->set_int(parent_index - 1, std::min(edge_v, cedge_v));
            }
            cedge->get_int(parent_index - 1, cedge_v);
            t->set_int(i, N);
            if (edge_v == cedge_v) {
                mask->set_int(i, 1);
                index->set_int(i, i + 1);
            }
        }

        for (int i = 0; i < N; ++i) {
            int parent_index, t_v, index_v;
            parent_->get_int(i, parent_index);
            t->get_int(parent_index - 1, t_v);
            index->get_int(i, index_v);
            t->set_int(parent_index - 1, std::min(t_v, index_v));
        }

        for (int i = 0; i < N; ++i) {
            int parent_index, t_v;
            parent_->get_int(i, parent_index);
            t->get_int(parent_index - 1, t_v);
            index->set_int(i, t_v);
        }

        spla::exec_v_eadd(parent_updated, parent_, edge, spla::MIN_INT, desc);
        std::swap(parent_, parent_updated);

        std::vector<spla::T_INT> bf1, bf2, bf3;

        auto rows_view = spla::MemView::make(bf1.data(), S_size->as_int(), false);
        auto cols_view = spla::MemView::make(bf2.data(), S_size->as_int(), false);
        auto values_view = spla::MemView::make(bf3.data(), S_size->as_int(), false);
        S->read(rows_view, cols_view, values_view);

        int* rows = (int*)rows_view->get_buffer();
        int* cols = (int*)cols_view->get_buffer();
        int* values = (int*)values_view->get_buffer();

        for (int i = 0; i < S_size->as_int(); ++i) {
            int row = rows[i];
            int col = cols[i];

            int row_parent, col_parent;
            parent_->get_int(row, row_parent);
            parent_->get_int(col, col_parent);

            if (row_parent != col_parent) {
                S_new->set_int(row, col, values[i]);
            }
        }
        std::swap(S, S_new);

        ReduceByVectors(S_size, S, spla::PLUS_INT, desc);
        S_empty = S_size->as_int() <= 0;
    }

    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

double const& MSTRunner::GetResult() const {
    return weight_;
}
