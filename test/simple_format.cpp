// Created by u7i

#include <cfmt.hpp>

#include <iostream>
#include <cassert>

int main() {
    // Basic features test
    assert(cfmt::format("{} {} {}", 1, "abcd", 1.5f) == "1 abcd 1.5");
    assert("{} {} {}"_cfmt(1, "abcd", 1.5f) == "1 abcd 1.5");

    std::cout << "Passed successfully: " << "{} {} {}"_cfmt(1, "abcd", 1.5f) << '\n';
    return 0;
}