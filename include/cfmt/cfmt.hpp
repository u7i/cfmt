// Created by u7i.

#pragma once

#include "formatter.hpp"

#include <sstream>
#include <vector>

namespace cfmt {
    namespace details {
        template <typename view_t> requires Indexable<view_t>
        struct cache {
            std::optional<view_t> find(const view_t& id) {
                for (const auto& [gid, gdata] : data_) {
                    if (compare(gid, id)) {
                        return { gdata };
                    }
                }

                return std::nullopt;
            }

            void push(const view_t& id, const view_t& data) {
                data_.emplace_back(id, data);
            }

        private:
            std::vector<std::pair<view_t, view_t>> data_;
        };
    }

    /** Basic implementation for the stdlib. */
    namespace std_details {
        template <typename string_t, typename stream_t>
        struct universal_string {
            using value_type = typename string_t::value_type;
            using size_type = size_t;

            universal_string() = default;
            universal_string(const auto& val) : str_(val) {}

            ~universal_string() = default;

            void append_view(const view<string_t>& view) {
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

    auto format = formatter<std_details::string> ("{%%}"); ///< Formatter for std::string
    auto wformat = formatter<std_details::wstring> (L"{%%}"); ///< Formatter for std::wstring
}

/** Create formatted std::string. */
auto operator "" _cfmt(const char* tstr, std::size_t) {
    return [tstr](const auto&... args) {
        return cfmt::format(tstr, args...);
    };
}