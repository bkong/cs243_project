#include <iostream>
#include <list>
#include <numeric>
#include <stdlib.h>
#include <boost/algorithm/cxx11/iota.hpp>

const size_t SIZE = 10000;

size_t test_lists() {
    std::list<size_t> li1(SIZE);

    boost::algorithm::iota(li1.begin(), li1.end(), 1);

    std::list<size_t> li2(li1);

    std::list<size_t> li3;

    size_t N = li2.size();
    while (N--) {
        li3.push_back(li2.front());
        li2.pop_front();
    }

    N = li3.size();
    while (N--) {
        li2.push_back(li3.back());
        li3.pop_back();
    }

    li1.reverse();

    return (li1.front() == SIZE) && (li1 == li2) ? li1.size() : 0;
}

int main(int argc, char* argv[]) {
    size_t ITER = (argc == 2 ? (atoi(argv[1]) < 1 ? 1 : atoi(argv[1])): 1);

    size_t result = 0;
    while (ITER > 0) {
        result = test_lists();
        --ITER;
    }

    std::cout << result << std::endl;
}

