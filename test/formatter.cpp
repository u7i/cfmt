// Created by u7i

#include "base.hpp"
#include <cfmt/formatter.hpp>

#include <sstream>

struct minimal_string {
    using size_type = size_t;
    using value_type = std::string::value_type;

    minimal_string() : str_() {}
    minimal_string(const auto& val) : str_(val) {}

    void append_block(const auto& view) {
        const size_t new_blk = str_.length();

        str_.resize(str_.length() + view.length());
        for (size_t i = 0; i < view.length(); ++i)
            str_[new_blk + i] = view[i];
    }

    void append_value(const auto& val) {
        str_ += (std::stringstream{} << val).str();
    }

    [[nodiscard]] auto length()             const noexcept -> size_t { return str_.length(); }
    [[nodiscard]] auto wrapped()            const noexcept -> const std::string& { return str_; }
    [[nodiscard]] auto operator[](size_t i) const          -> char { return str_[i]; }

private:
    std::string str_;
};

int main() {
    section("formatter");
    {
        const cfmt::formatter<minimal_string> formatter{cfmt::group::blueprint<minimal_string>{"{", "}"}};

        test_compare("group-less string", formatter("no format"), "no format");
        test_compare("anonymous groups", formatter("{} {} {}", 1, "abcd", 1.5f), "1 abcd 1.5");
        test_compare("mixed", formatter("{a} {b} {a} {}", 1, "w", 4), "1 w 1 4");
    }

    return 0;
}