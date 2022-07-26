// Created by u7i.

#pragma once

#include "formatter.hpp"
#include <sstream>

namespace cfmt {
    /** Basic implementation for the stdlib. */
    namespace impl {
        template <typename string_t, typename stream_t>
        struct universal_string {
            using value_type = typename string_t::value_type;
            using size_type = size_t;

            universal_string() = default;
            universal_string(const auto& val = {}) : str_(val) {}

            template<typename T> requires Indexable<T>
            void append_block(const T& view) {
                const size_t new_blk = str_.length();
                str_.resize(str_.length() + view.length());

                for (size_t i = 0; i < view.length(); ++i)
                    str_[new_blk + i] = view[i];
            }

            void append_value(const auto& val) {
                str_ += (stream_t{} << val).str();
            }

            [[nodiscard]] size_t   length()  const noexcept { return str_.length(); }
            [[nodiscard]] string_t wrapped() const noexcept { return str_; }

            [[nodiscard]] auto operator[](size_t i) const { return str_[i]; }
        private:
            string_t str_;
        };

        using string = universal_string<std::string, std::stringstream>;
        using wstring = universal_string<std::wstring, std::wstringstream>;
    }

    auto format = formatter<impl::string> ({"{", "}"}); ///< Formatter for std::string
    auto wformat = formatter<impl::wstring> ({L"{", L"}"}); ///< Formatter for std::wstring
}

/** Create formatted std::string. */
auto operator "" _cfmt(const char* tstr, std::size_t) {
    return [tstr](const auto&... args) {
        return cfmt::format(tstr, args...);
    };
}