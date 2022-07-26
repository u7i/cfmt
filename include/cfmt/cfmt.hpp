// Created by u7i.

#pragma once

#include "indexable.hpp"

#include <concepts>
#include <sstream>
#include <vector>
#include <utility>
#include <optional>
#include <exception>

#include <iostream> // TODO:

void debug_iterable(const auto& obj) {
    for (int i = 0; i < obj.length(); ++i)
        std::cout << obj[i];
    std::cout << "| end (" << obj.length() << ")\n";
}

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

        template <typename view_t, typename string_t>
        class formatter_impl {
            using size_type = typename string_t::size_type;

        public:
            formatter_impl(const sign_t& sign, const view_t& tstr) : sign_(sign), tstr_(tstr) {}

            template<typename... args_t>
            string_t run(const args_t&... args) {
                format_args(std::forward<const args_t&>(args)...); // format the main-part
                out_.append_view(tstr_); // append the rest

                return out_;
            }

        private:
            template <typename T, typename... args_t>
            void format_args(const T& val, const args_t&... args) {
                // Parse the closest group
                auto sign = extract_group_id(); // TODO: rename as find_group()
                if (sign) {
                    [&] {
                        view_t id = sign.value().second;
                        out_.append_view(sign.value().first); // TODO: Separate as gap_append()
                        // TODO: Add try_process_valueless() -> bool
                        // TODO: Add process_with_value(val) -> void

                        if (id.length() == 0) {
                            out_.append_value(val); // if group name is unknown
                            return;
                        }

                        // Try to find value in the cache
                        auto cached_val = cache_.find(id);
                        if (cached_val) {
                            out_.append_view(cached_val.value());
                            return;
                        }

                        // Ok, lets add this value to the cache // TODO: Move to try_process_valueless()
                        const size_type first = out_.length();
                        out_.append_value(val);
                        cache_.push(id, out_.view().shrink(first, out_.length() - first));
                    }();

                    format_args(args...);
                }
            }

            void format_args() {
                auto sign = extract_group_id();
                if (sign) {
                    if (sign.value().second.length() != 0) { // TODO: Make a class for a group ( is_unnamed() )
                        out_.append_view(sign.value().first);

                        // TODO: Move to try_process_valueless();
                        auto cached = cache_.find(sign.value().first);
                    }

                    throw std::out_of_range("Not enough arguments given");
                }
            }

            std::optional<std::pair<view_t, view_t>> extract_group_id() {
                // TODO: Move all dropping to the drop_group(group)

                auto old = tstr_;
                auto open = extract_first_pivot(sign_.open);
                auto close = extract_first_pivot(sign_.close);

                if (open && close) {
                    size_type id = open.value() + sign_.open.length();
                    std::cout << "WTF = " << id << ' ' << close.value() << '\n';

                    return { { old.shrink(0, open.value()), old.shrink(id, (close.value() + sign_.open.length()) - id) } };
                }

                return std::nullopt;
            }

            std::optional<size_type> extract_first_pivot(const auto& needle) {
                auto pos = find(tstr_, needle);
                if (pos) {
                    size_type needle_end = pos.value() + needle.length();
                    tstr_ = tstr_.shrink(needle_end, tstr_.length() - needle_end); // TODO: Mustn't do any-shrinking, reimplement using the pos parameter
                }

                return pos;
            }

            const sign_t& sign_;
            view_t tstr_;

            string_t out_;
            cache<view_t> cache_;
        };
    }

    /**
     * String formatter facade.
     * \details Can be easily adapted to any string class using "adapter" class.
     *
     * @tparam adapter String adapter instance ( must be compatible with string_t ).
     */
    template <typename adapter_t>
    struct formatter {
        using view_type   = typename adapter_t::view_type;
        using string_type = typename adapter_t::string_type;

        formatter(string_type sign) {
            // Try to parse sign
        }
        ~formatter() = default;

        [[nodiscard]] auto sign() const noexcept -> const string_type& { return sign_; }; ///< Get the current sign.

        /** Format template string with the args. */
        template <typename... args_t>
        auto operator()(const auto& tstr, const args_t&... args) const {
            auto impl = details::formatter_impl<view_type, string_type, sign_type> {sign_, view_type { tstr }};
            return impl.run(std::forward<const args_t&>(args)...).wrapped();
        }

    private:
        
    };

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