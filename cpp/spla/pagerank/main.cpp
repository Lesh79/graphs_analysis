#include <iostream>
#include <spla.hpp>

void print_m(spla::ref_ptr<spla::Matrix> const& m, std::string const& name) {
    std::vector<int> bf1, bf2, bf3;
    std::cout << "-- " << name << " --\n";
    auto keys1_view = spla::MemView::make(bf1.data(), 4, false);
    auto keys2_view = spla::MemView::make(bf2.data(), 4, false);
    auto values_view = spla::MemView::make(bf3.data(), 4, false);

    m->read(keys1_view, keys2_view, values_view);
    auto keys1 = (int*)keys1_view->get_buffer();
    auto keys2 = (int*)keys2_view->get_buffer();
    auto values = (int*)values_view->get_buffer();
    for (unsigned int i = 0; i < 4; i++) {
        std::cout << keys1[i] << ' ' << keys2[i] << " | " << values[i] << '\n';
    }
}

auto ZERO = spla::Scalar::make_int(0);
auto ONE = spla::Scalar::make_int(1);

int main() {
    auto m1 = spla::Matrix::make(2, 2, spla::INT);
    auto m2 = spla::Matrix::make(2, 2, spla::INT);
    auto mr = spla::Matrix::make(2, 2, spla::INT);
    auto mask = spla::Matrix::make(2, 2, spla::INT);

    m1->set_fill_value(ZERO);
    m2->set_fill_value(ZERO);
    mr->set_fill_value(ZERO);
    mask->set_fill_value(ZERO);

    m1->set_int(0, 0, 1);
    m1->set_int(1, 1, 1);
    m1->set_int(0, 1, 1);
    m1->set_int(1, 0, 1);

    m2->set_int(0, 0, 1);
    m2->set_int(0, 1, 1);
    m2->set_int(1, 0, 1);
    m2->set_int(1, 1, 1);

    mask->set_int(0, 0, 1);
    mask->set_int(0, 1, 1);
    mask->set_int(1, 0, 1);
    mask->set_int(1, 1, 1);

    print_m(m1, "m1");
    print_m(m2, "m2");
    print_m(mask, "mask");

    spla::exec_mxmT_masked(mr, mask, m1, m2, spla::FIRST_INT, spla::SECOND_INT, spla::ALWAYS_INT,
                           ZERO);

    print_m(mr, "mr");

    return 0;
}