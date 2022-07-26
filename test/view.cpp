// Created by u7i
#include "cfmt/view.hpp"

#include <iostream>

void test_case(const std::string& name, bool result) {
    if (result)
        std::cout << "Successfully passed: " << name << std::endl;
    else
        std::cerr << "Failed: " << name << std::endl;
}

void test_compare(const std::string& name, const auto& input, const std::string& expected) {
    test_case(name, cfmt::indexable::compare(input, expected));
}

int main() {
    // Basic features test
    const std::string string = "abc bc db";
    const cfmt::view view = string;

    test_compare("shift by 2", view.shift(2), string.substr(2));
    test_compare("shrink", view.shrink(2, 3), string.substr(2, 3));
    test_case("length", view.length() == string.length());

    test_case("find", cfmt::indexable::find(view, string.substr(2, 4)).has_value());
    test_case("find index", cfmt::indexable::find(view, string.substr(2, 4)).value() == 2);

    return 0;
}