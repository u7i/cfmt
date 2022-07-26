// Created by u7i

#include "base.hpp"
#include <cfmt/view.hpp>

int main() {
    // Basic features test
    const std::string string = "abc bc db";
    const cfmt::view view = string;

    section("basic features");
    {
        test_compare("shift by 2", view.shift(2), string.substr(2));
        test_compare("shrink", view.shrink(2, 3), string.substr(2, 3));
        test_case("length", view.length() == string.length());
    }

    section("indexable find");
    {
        test_case("find", cfmt::indexable::find(view, string.substr(2, 4)).has_value());
        test_case("find index", cfmt::indexable::find(view, string.substr(2, 4)).value() == 2);
    }

    return 0;
}