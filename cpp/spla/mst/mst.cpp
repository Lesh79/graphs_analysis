#include "mst.h"

#include <chrono>

int MSTRunner::RunAlgo(SPLAGraph const& graph) {
    int N = graph.n_vertices;

    spla::ref_ptr<spla::Descriptor> desc = spla::Descriptor::make();
    desc->set_early_exit(true);
    desc->set_struct_only(true);

    spanning_tree_ = spla::Matrix::make(N, N, spla::INT);

    spla::ref_ptr<spla::Vector> edge = spla::Vector::make(N, spla::INT);
    spla::ref_ptr<spla::Vector> parent_ = spla::Vector::make(N, spla::INT);
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

    spla::ref_ptr<spla::Vector> parent_updated = spla::Vector::make(N, spla::INT);

    auto start = std::chrono::high_resolution_clock::now();

    while (!S_empty) {
        S_new->clear();

        spla::exec_mxv_masked(edge, full_mask, S, parent_, spla::SECOND_INT, spla::MAX_INT,
                              spla::ALWAYS_INT, SPLA_ZERO_INT, desc);

        spla::exec_v_eadd(parent_updated, parent_, edge, spla::MIN_INT, desc);

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
            parent_updated->get_int(row, row_parent);
            parent_updated->get_int(col, col_parent);

            int edge_v, parent_v;
            edge->get_int(row, edge_v);
            parent_->get_int(col, parent_v);

            if (edge_v == parent_v) {
                spanning_tree_->set_int(row, col, values[i]);
            }

            if (row_parent != col_parent) {
                S_new->set_int(row, col, values[i]);
            }
        }
        std::swap(parent_, parent_updated);
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
