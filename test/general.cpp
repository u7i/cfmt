
#include "base.hpp"
#include <cfmt/cfmt.hpp>

int main() {
    section("cfmt");
    {
        test_compare("group-less string", "no format"_cfmt(), "no format");
        test_compare("anonymous", "{} {} {}"_cfmt(1, "abcd", 1.5f), "1 abcd 1.5");
        test_compare("single named group", "{a} {a}"_cfmt(1), "1 1");
        test_compare("mixed", "{a} {b} {a} {}"_cfmt(1, "w", 4), "1 w 1 4");
    }
}