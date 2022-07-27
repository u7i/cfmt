// Created by u7i.

#include "view.hpp"
#include "string.hpp"
#include "group/cache.hpp"
#include "group/blueprint.hpp"

namespace cfmt {
    /**
     * String formatter facade.
     * \details Requires only string that compatible with String concept.
     *
     * @tparam string String type.
     */
    template <typename string> requires String<string>
    struct formatter {
        using blueprint_type = group::blueprint<string>;
        using cache_type = group::cache<string>;
        using view_type = view<string>;

        formatter(const blueprint_type& bp) noexcept : blueprint_(bp) {}

        /** Set new group blueprint. */
        void set_blueprint(const blueprint_type& bp) noexcept {
            blueprint_ = bp;
        }

        /** Get the current group blueprint. */
        [[nodiscard]] auto blueprint() const noexcept -> const blueprint_type&
        {
            return blueprint_;
        };

        /** Format template string with the args. */
        template <typename... args_t>
        auto operator()(const string& tstr, const args_t&... args) const {
            string out;
            cache_type cache;

            format({
                .tstr = tstr,
                .cache = cache,
                .out = out
            }, std::forward<const args_t&>(args)...);

            return out.wrapped();
        }

    private:
        struct context {
            view_type tstr;
            cache_type& cache;
            string& out;
        };

        void format(context ctx) const {
            auto match = blueprint_.extract(ctx.tstr);

            if (match.valid()) {
                if (match.anonymous() || !try_use_cached(match, ctx))
                    throw std::out_of_range("Not enough args given");

                format(ctx);
            }
            else {
                use_rest(ctx);
            }
        }

        template <typename T, typename... args>
        void format(context ctx, const T& val, const args&... tail) const {
            auto match = blueprint_.extract(ctx.tstr);

            if (match.valid()) {
                if (match.anonymous()) {
                    use_value(match, val, ctx);
                    format(ctx, std::forward<const args&>(tail)...);
                }
                else if (try_use_cached(match, ctx)) {
                    format(ctx, val, std::forward<const args &>(tail)...);
                }
                else {
                    use_named_value(match, val, ctx);
                    format(ctx, std::forward<const args&>(tail)...);
                }
            }
            else {
                use_rest(ctx);
            }
        }

        [[nodiscard]] auto try_use_cached(const group::match<string>& match, context& ctx) const -> bool
        {
            auto cached = ctx.cache.find(match.id());
            if (!cached)
                return false;

            ctx.out.append_block(match.prefix());
            ctx.out.append_block(cached.value());

            ctx.tstr = match.suffix();
            return true;
        }

        void use_value(const group::match<string>& match, const auto& value, context& ctx) const {
            ctx.out.append_block(match.prefix());
            ctx.out.append_value(value);

            ctx.tstr = match.suffix();
        }

        void use_named_value(const group::match<string>& match, const auto& value, context& ctx) const {
            ctx.out.append_block(match.prefix());

            const auto old = ctx.out.length();
            ctx.out.append_value(value);
            ctx.cache.push(match.id(), view_type { old, ctx.out.length() - old, ctx.out });

            ctx.tstr = match.suffix();
        }

        void use_rest(context& ctx) const {
            if (!ctx.tstr.empty())
                ctx.out.append_block(ctx.tstr);
        }

        blueprint_type blueprint_;
    };
}