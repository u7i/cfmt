// Created by u7i.

#pragma once

#include <concepts>
#include <sstream>
#include <vector>
#include <utility>
#include <optional>

#include <iostream> // TODO:

void debug_iterable(const auto& obj) {
    for (int i = 0; i < obj.length(); ++i)
        std::cout << obj[i];
    std::cout << "| end (" << obj.length() << ")\n";
}

namespace cfmt {
    /** Abstract indexable object. */
    template <class T>
    concept Indexable = requires(T a) {
        a.length();
        a.operator[](0);
    };
    
    template <typename string_t>
    struct placeholder_sign {
        placeholder_sign(string_t open, string_t close) : open(std::move(open)), close(std::move(close)) {}

        string_t open;
        string_t close;
    };

    namespace details {
        template <class base_t> requires Indexable<base_t>
        struct advanced_view : public base_t {
            using size_type = typename base_t::size_type;

            std::optional<size_type> find(const base_t &needle, size_type pos) noexcept {
                if (needle.length() > this->length() || needle.length() == 0)
                    return std::nullopt;

                // Naive find implementation
                const size_type shift = needle.length() - 1;

                for (size_type c = pos, i = 0; c < this->length() - shift; ++c) {
                    i = needle[i] == this->operator[](c) ? i + 1 : 0;

                    if (i == needle.length())
                        return {c - shift};
                }

                return std::nullopt;
            }

            bool operator==(const base_t &other) const noexcept {
                if (this->length() != other.length())
                    return false;

                for (size_t i = 0; i < this->length(); ++i) {
                    if (this->operator[](i) != other[i])
                        return false;
                }

                return true;
            }
        };

        template <typename view_t>
        struct cache {
            std::optional<view_t> find(const advanced_view<view_t>& id) {
                for (const auto &[gid, gdata]: data_) {
                    if (id.compare(gid)) {
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
            using sign_type = placeholder_sign<string_t>;
            using size_type = typename string_t::size_type;

        public:
            formatter_impl(const sign_type& sign, const view_t& tstr) : sign_(sign), tstr_(tstr) {}

            template<typename... args_t>
            string_t run(const args_t&... args) {
                format_args(std::forward<const args_t&>(args)...); // format the main-part
                out_.append(tstr_); // append the rest

                return out_;
            }

        private:
            void format_args() {
                auto sign = extract_group_id();
                if (sign) {
                    if (sign.value().second.length() != 0) { // TODO: Make a class for a group ( is_unnamed() )
                        out_.append(sign.value().first);

                        // TODO: Move to try_process_valueless();
                        auto cached = cache_.find(sign.value().first);
                    }

                    throw std::out_of_range("Not enough arguments given");
                }
            }

            template <typename T, typename... args_t>
            void format_args(const T& val, const args_t&... args) {
                // Find pivots
                auto sign = extract_group_id(); // TODO: rename as find_group()
                if (sign) {
                    [&] {
                        view_t id = sign.value().second;
                        out_.append(sign.value().first); // TODO: Separate as gap_append()
                        // TODO: Add try_process_valueless() -> bool
                        // TODO: Add process_with_value(val) -> void

                        if (id.length() == 0) {
                            out_.append(val); // if group name is unknown
                            return;
                        }

                        // Try to find value in the cache
                        for (const auto &[gid, gdata]: cache_) {
                            if (compare(gid, id)) {
                                out_.append(gdata);
                                return;
                            }
                        }

                        // Ok, lets add this value to the cache // TODO: Move to try_process_valueless()
                        const size_type first = out_.length();
                        out_.append(val);

                        cache_.emplace_back(id, out_.view().shrink(first, out_.length() - first));
                    }();

                    format_args(args...);
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

            const sign_type& sign_;
            advanced_view<view_t> tstr_;

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
        using sign_type   = placeholder_sign<string_type>;

        explicit formatter(sign_type sign) noexcept : sign_(std::move(sign)) {}
        ~formatter() = default;

        void set_sign(const sign_type& sign) noexcept { sign_ = sign; }; ///< Set a new sign.
        [[nodiscard]] const sign_type& sign() const noexcept { return sign_; }; ///< Get the current sign.

        /** Format template string with the args. */
        template <typename... args_t>
        auto operator()(const auto& tstr, const args_t&... args) const {
            auto impl = details::formatter_impl<view_type, string_type> {sign_, view_type { tstr }};
            return impl.run(std::forward<const args_t&>(args)...).wrapped();
        }

    private:
        sign_type sign_;
    };

    /** Basic implementation for the stdlib. */
    namespace std_details {
        template <typename string_t>
        struct string_view {
            using size_type = size_t;

            string_view(const string_t& str) : first_(0), length_(str.length()), str_(&str) {}
            string_view(size_t first, size_t length, const string_t& str) : first_(first), length_(length), str_(&str) {}

            [[nodiscard]] auto length()                            const noexcept { return length_; }
            [[nodiscard]] auto shrink(size_t first, size_t length) const          { return string_view<string_t>(first_ + first, length, *str_); }

            [[nodiscard]] auto operator[](size_t i) const { return (*str_)[first_ + i]; }

        private:
            size_t first_, length_;
            const string_t* str_;
        };

        template <typename string_t, typename stream_t>
        struct universal_string {
            using size_type = size_t;

            universal_string() = default;
            universal_string(const auto& val) : str_(val) {}

            ~universal_string() = default;

            void append(const string_view<string_t>& view) {
                const size_t new_blk = str_.length();
                str_.resize(str_.length() + view.length());

                for (size_t i = 0; i < view.length(); ++i)
                    str_[new_blk + i] = view[i];
            }

            void append(const auto& val) {
                str_ += (stream_t{} << val).str();
            }

            [[nodiscard]] size_t                length()  const noexcept { return str_.length(); }
            [[nodiscard]] string_view<string_t> view()    const noexcept { return { str_ }; }
            [[nodiscard]] string_t              wrapped() const noexcept { return str_; }

            [[nodiscard]] auto operator[](size_t i) const { return str_[i]; }
        private:
            string_t str_;
        };

        /** Universal adapter for strings with stdlib style API. */
        template <typename string_t, typename stream_t>
        struct adapter {
            using string_type  = universal_string<string_t, stream_t>;
            using view_type    = string_view<string_t>;
        };

        using string = adapter<std::string, std::stringstream>;
        using wstring = adapter<std::wstring, std::wstringstream>;
    }

    auto format = formatter<std_details::string> ({"{", "}"}); ///< Formatter for std::string
    auto wformat = formatter<std_details::wstring> {{L"{", L"}"}}; ///< Formatter for std::wstring
}

/** Create formatted std::string. */
auto operator "" _cfmt(const char* tstr, std::size_t) {
    return [tstr](const auto&... args) {
        return cfmt::format(tstr, args...);
    };
}