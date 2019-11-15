#include <pybind11/pybind11.h>
#include <iostream>
#include <string>
#include <vector>

namespace py = pybind11;

void foo(long long int num) {
    std::vector<int> grid;
    long long int hex = 0xf;
    int shift = 0;
    for(int i = 0; i < 16; i++) {
        grid.insert(grid.begin(), (num & hex) >> shift);
        hex *= 0xf + 1;
        shift += 4;
    }
    for(int i = 0; i < grid.size(); i++)
        std::cout << grid[i] << ' ';
}

int add(int i, int j) {
    return i + j;
}

PYBIND11_MODULE(example, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    m.def("add", &add, "A function which adds two numbers");
    m.def("foo", &foo, "A function which adds two numbers");

}