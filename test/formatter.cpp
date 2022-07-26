// Created by u7i

#include "base.hpp"
#include <cfmt/formatter.hpp>

int main() {
    // Basic features test
    test_compare("group-less string", cfmt::format("no format"), "no format");
    test_compare("anonymous groups", cfmt::format("{} {} {}", 1, "abcd", 1.5f), "1 abcd 1.5");
    test_compare("named groups", cfmt::format("{a} {b} {a} {}", 1, "w", 4), "1 w 1 4");

    // Groups feature
    //test_case("{} {} {}"_cfmt(1, "abcd", 1.5f), "1 abcd 1.5");
    //test_case("{a} {a}"_cfmt(1), "1 1");
    //test_case("{a} {b} {a} {}"_cfmt(1, "w", 4), "1 w 1 4");

    return 0;
}