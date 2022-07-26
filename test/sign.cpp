// Created by u7i
#include "cfmt/sign/blueprint.hpp"

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

using namespace cfmt::sign;

int main() {
    const std::string signless = "there is no sign",
                      anonymous = "p {} s",
                      named = "p1 {abc} s1";

    const blueprint<std::string> bp { "{", "}" };
    
    test_case("signless", !bp.extract(signless).valid());
    test_exception<bad_match>("signless id access", [&] { (void)bp.extract(signless).id(); });
    test_exception<bad_match>(R"(signless "is anonymous" access)", [&] { (void)bp.extract(signless).anonymous(); });

    test_case("anonymous", bp.extract(anonymous).anonymous());
    test_compare("anonymous prefix", bp.extract(anonymous).prefix(), "p ");
    test_compare("anonymous suffix", bp.extract(anonymous).suffix(), " s" );

    test_case("named", !bp.extract(named).anonymous());
    test_compare("named gid", bp.extract(named).id(), "abc");
    test_compare("named prefix", bp.extract(named).prefix(), "p1 ");
    test_compare("named suffix", bp.extract(named).suffix(), " s1");

    return 0;
}

