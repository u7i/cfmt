// Created by u7i.

#pragma once

#include <concepts>
#include <sstream>

#define cfmt_setter(member, v) { (member) = v; } ///< Trivial getter body.
#define cfmt_getter(member) { return member; } ///< Trivial setter body.

namespace cfmt {
    /** Abstract adapter constraints. */
    template <class T, class string_t>
    concept Adapter = requires(T a, string_t str) {
        { a.append(str, str) }                -> std::same_as<void>;
        { a.find(str, size_t{}, str) }        -> std::same_as<size_t>;
        { a.substr(size_t{}, size_t{}, str) } -> std::same_as<string_t>;
        { a.size(str) }                       -> std::same_as<size_t>;
        { a.to_string(0) }                    -> std::same_as<string_t>;
    };

    /**
     * String formatter logic.
     * \details Can be easily adapted to any string class using adapters.
     *
     * @tparam string_t Type of the string.
     * @tparam adapter String adapter instance ( must be compatible with string_t ).
     */
    template <typename string_t, auto adapter> requires Adapter<decltype(adapter), string_t>
    struct formatter {
        using ref = string_t&;
        using const_ref = const string_t&;

        formatter(const_ref sign) noexcept : sign_(sign) {}
        ~formatter() = default;

        void      set_sign(const_ref sign)       noexcept cfmt_setter(sign_, sign); ///< Set new sign
        const_ref sign()                   const noexcept cfmt_getter(sign_); ///< Get the current sign

        /** Format template string with the args. */
        template <typename... args_t>
        string_t operator()(const_ref tstr, const args_t&... args) const {
            string_t out;
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
        }

        template <typename T, typename... args_t>
        static void format(format_ctx ctx, const T& val, const args_t&... args) {
            auto pos = adapter.find(ctx.sign, ctx.i, ctx.tstr);

            if (pos != -1) {
                if (pos != ctx.i)
                    adapter.append(ctx.out, adapter.substr(ctx.i, pos - 1, ctx.tstr));

                adapter.append(ctx.out, adapter.to_string(val));

                // Launch recursion
                ctx.i = pos + adapter.size(ctx.sign);
                if (ctx.i < adapter.size(ctx.tstr))
                    format(ctx, args...);
            }
        }

        string_t sign_;
    };

    /** Basic implementation for stdlib. */
    namespace std_adapters {
        /** Universal adapter for strings with stdlib style API. */
        template <typename builder_t>
        struct base_adapter {
            void append(auto& target, const auto& tail) const {
                target += tail;
            }

            [[nodiscard]] size_t find(const auto& needle, size_t pos, const auto& str) const {
                return str.find(needle, pos);
            }

            [[nodiscard]] auto substr(size_t begin, size_t last, const auto& str) const {
                return str.substr(begin, last - begin + 1);
            }

            [[nodiscard]] size_t size(const auto& str) const {
                return str.size();
            }

            [[nodiscard]] auto to_string(const auto& any) const {
                builder_t str;
                str << any;

                return str.str();
            }
        };

        using string = base_adapter<std::stringstream>;
        using wstring = base_adapter<std::wstringstream>;
    }

    auto format = formatter<std::string, std_adapters::string{}> {"{}"}; ///< Formatter for std::string
    auto wformat = formatter<std::wstring, std_adapters::wstring{}> {L"{}"}; ///< Formatter for std::wstring
}

/** Create formatted std::string. */
auto operator "" _cfmt(const char* tstr, std::size_t) {
    return [tstr](const auto&... args) {
        return cfmt::format(tstr, args...);
    };
}