// Created by u7i

#include <cfmt/indexable.hpp>

#include <string>
#include <iostream>

/** Print new test section. */
void section(const std::string& name) {
    std::cout << "\x1B[34m| ---- " << name << " ----\033[0m" << std::endl;
}

/** Base test case ( boolean expression ). */
void test_case(const std::string& name, bool result) {
    std::cout << (result ? "\x1B[32mSuccessfully passed: " : "\x1B[31mFailed: ") << name << "\033[0m" << std::endl;
}

/** Test that requires exact output comparing. */
void test_compare(const std::string& name, const auto& input, const std::string& expected) {
    test_case(name, cfmt::indexable::compare(input, expected));
}

/** Test that requires exception to be thrown. */
template <typename exception>
void test_exception(const std::string& name, auto expression) {
    try {
        expression();
        test_case(name, false);
    }
    catch (const exception& ex) {
        test_case(name, true);
    }
}

/** Print indexable object. */
template <typename indexable, typename size_type = typename indexable::size_type> requires cfmt::Indexable<indexable>
void debug_indexable(const indexable& str) {
    for (size_type i = 0; i < str.length(); ++i)
        std::cout << str[i];
    std::cout << '\n';
}