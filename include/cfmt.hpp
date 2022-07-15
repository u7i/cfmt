// Created by u7i.

#pragma once

#include <concepts>
#include <sstream>

namespace cfmt {
    /** Abstract adapter constraints. */
    template <class T>
    concept Adapter = requires(T a, typename T::string_type str) {
        { a.create() }                        -> std::same_as<decltype(str)>; ///< Create new string.
        { a.append(str, str) }                -> std::same_as<void>; ///< Append one string to another.
        { a.find(str, size_t{}, str) }        -> std::same_as<size_t>; ///< Find a substring, if not found -> -1.
        { a.substr(size_t{}, size_t{}, str) } -> std::same_as<decltype(str)>; ///< Get a substring.
        { a.size(str) }                       -> std::same_as<size_t>; ///< Get size of a string.
        { a.to_string(0) }                    -> std::same_as<decltype(str)>; ///< Convert variable to string.
    };

    /**
     * String formatter logic.
     * \details Can be easily adapted to any string class using adapters.
     *
     * @tparam adapter String adapter instance ( must be compatible with string_t ).
     */
    template <auto adapter> requires Adapter<decltype(adapter)>
    struct formatter {
        using string_t  = typename decltype(adapter)::string_type; ///< Type of the formatter string.
        using ref       = string_t&; ///< String reference.
        using const_ref = const string_t&; ///< Const string reference.

        formatter(const_ref sign) noexcept : sign_(sign) {}
        ~formatter() = default;

        void set_sign(const_ref sign) noexcept { sign_ = sign; }; ///< Set a new sign
        [[nodiscard]] const_ref sign() const noexcept { return sign_; }; ///< Get the current sign

        /** Format template string with the args. */
        template <typename... args_t>
        string_t operator()(const_ref tstr, const args_t&... args) const {
            string_t out = adapter.create();
            format({
               .i = 0,
               .tstr = tstr,
               .sign = sign_,
               .out = out
            }, args...);

            return out;
        }

    private:
        struct format_ctx {
            int i;
            const_ref tstr;
            const_ref sign;

            ref out;
        };

        static void format(format_ctx ctx) {
            if (adapter.find(ctx.sign, ctx.i, ctx.tstr) != -1)
                throw std::out_of_range("Not enough args given");

            append_rest(ctx);
        }

        template <typename T, typename... args_t>
        static void format(format_ctx ctx, const T& val, const args_t&... args) {
            auto pos = adapter.find(ctx.sign, ctx.i, ctx.tstr);
            if (pos == -1) {
                append_rest(ctx);
                return;
            }

            if (pos != ctx.i)
                adapter.append(ctx.out, adapter.substr(ctx.i, pos - 1, ctx.tstr));

            adapter.append(ctx.out, adapter.to_string(val));

            // Launch recursion
            ctx.i = pos + adapter.size(ctx.sign);
            if (ctx.i < adapter.size(ctx.tstr))
                format(ctx, args...);
        }

        static void append_rest(format_ctx ctx) {
            adapter.append(ctx.out, adapter.substr(ctx.i, adapter.size(ctx.tstr) - 1, ctx.tstr));
        }

        string_t sign_;
    };

    /** Basic implementation for stdlib. */
    namespace std_adapters {
        /** Universal adapter for strings with stdlib style API. */
        template <typename string_t, typename builder_t>
        struct base_adapter {
            using string_type = string_t;

            [[nodiscard]] string_t create() const {
                return {};
            }

            void append(string_t& target, const string_t& tail) const {
                target += tail;
            }

            [[nodiscard]] size_t find(const string_t& needle, size_t pos, const string_t& str) const {
                return str.find(needle, pos);
            }

            [[nodiscard]] auto substr(size_t begin, size_t last, const string_t& str) const {
                return str.substr(begin, last - begin + 1);
            }

            [[nodiscard]] size_t size(const string_t& str) const {
                return str.size();
            }

            [[nodiscard]] auto to_string(const auto& any) const {
                builder_t str;
                str << any;

                return str.str();
            }
        };

        using string = base_adapter<std::string, std::stringstream>;
        using wstring = base_adapter<std::wstring, std::wstringstream>;
    }

    auto format = formatter<std_adapters::string{}> {"{}"}; ///< Formatter for std::string
    auto wformat = formatter<std_adapters::wstring{}> {L"{}"}; ///< Formatter for std::wstring
}

/** Create formatted std::string. */
auto operator "" _cfmt(const char* tstr, std::size_t) {
    return [tstr](const auto&... args) {
        return cfmt::format(tstr, args...);
    };
}