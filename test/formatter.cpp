// Created by u7i
#include "cfmt/cfmt.hpp"

#include <iostream>
#include <cassert>

void test_case(const std::string& formatted, const std::string& expected) {
    assert(formatted == expected);
    std::cout << "Successfully passed: " << formatted << '\n';
}

int main() {
    // Basic features test
    test_case(cfmt::format("no format"), "no format");
    test_case(cfmt::format("{} {} {}", 1, "abcd", 1.5f), "1 abcd 1.5");
    test_case("{} {} {}"_cfmt(1, "abcd", 1.5f), "1 abcd 1.5");

    // Groups feature
    test_case("{a} {a}"_cfmt(1), "1 1");
    test_case("{a} {b} {a} {}"_cfmt(1, "w", 4), "1 w 1 4");

    return 0;
}