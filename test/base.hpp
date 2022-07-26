// Created by u7i

#include <cfmt/indexable.hpp>

#include <string>
#include <iostream>

void section(const std::string& name) {
    std::cout << "\x1B[34m| ---- " << name << " ----\033[0m" << std::endl;
}

void test_case(const std::string& name, bool result) {
    std::cout << (result ? "\x1B[32mSuccessfully passed: " : "\x1B[31mFailed: ") << name << "\033[0m" << std::endl;
}

void test_compare(const std::string& name, const auto& input, const std::string& expected) {
    test_case(name, cfmt::indexable::compare(input, expected));
}

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
