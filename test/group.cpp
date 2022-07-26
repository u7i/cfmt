// Created by u7i

#include "base.hpp"
#include <cfmt/group/blueprint.hpp>

using namespace cfmt::group;

int main() {
    const std::string groupless = "there is no group",
                      anonymous = "p {} s",
                      named = "p1 {abc} s1";

    const blueprint<std::string> bp { "{", "}" };

    section("group-less");
    {
        test_case("group-less", !bp.extract(groupless).valid());
        test_exception<bad_match_access>("group-less id access", [&] { (void) bp.extract(groupless).id(); });
        test_exception<bad_match_access>(R"(group-less "is anonymous" access)", [&] { (void) bp.extract(groupless).anonymous(); });
    }

    section("anonymous group");
    {
        test_case("anonymous", bp.extract(anonymous).anonymous());
        test_compare("anonymous prefix", bp.extract(anonymous).prefix(), "p ");
        test_compare("anonymous suffix", bp.extract(anonymous).suffix(), " s");
    }

    section("named group");
    {
        test_case("named", !bp.extract(named).anonymous());
        test_compare("named gid", bp.extract(named).id(), "abc");
        test_compare("named prefix", bp.extract(named).prefix(), "p1 ");
        test_compare("named suffix", bp.extract(named).suffix(), " s1");
    }

    return 0;
}