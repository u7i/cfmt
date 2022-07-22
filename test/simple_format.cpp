// Created by u7i

#include <cfmt.hpp>

#include <iostream>
#include <cassert>

int main() {
    // Basic features test
    assert(cfmt::format("no format") == "no format");
    assert(cfmt::format("{} {} {}", 1, "abcd", 1.5f) == "1 abcd 1.5");
    assert("{} {} {}"_cfmt(1, "abcd", 1.5f) == "1 abcd 1.5");
    assert("{a} {a}"_cfmt(1) == "1 1");

    std::cout << "Passed successfully: " << "{} {} {}"_cfmt(1, "abcd", 1.5f) << '\n';
    return 0;
}