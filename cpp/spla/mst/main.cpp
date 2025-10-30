#include <iostream>
#include <spla.hpp>

int main() {
    std::cout << "Hello world" << std::endl;
    spla::Library* library = spla::Library::get();
    std::string acc_info;
    library->get_accelerator_info(acc_info);
    std::cout << "env: " << acc_info << std::endl;
    return 0;
}