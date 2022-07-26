// Created by u7i.

#include "view.hpp"
#include "group/blueprint.hpp"

namespace cfmt {
    /**
     * String formatter facade.
     * \details Requires only string
     *
     * @tparam string T
     */
    template <typename string> requires Indexable<string>
    struct formatter {
        using view_type   = typename adapter_t::view_type;
        using string_type = typename adapter_t::string_type;

        formatter(string_type group) {
            // Try to parse group
        }
        ~formatter() = default;

        [[nodiscard]] auto group() const noexcept -> const string_type& { return group_; }; ///< Get the current group.

        /** Format template string with the args. */
        template <typename... args_t>
        auto operator()(const auto& tstr, const args_t&... args) const {
            auto impl = details::formatter_impl<view_type, string_type, group_type> {group_, view_type { tstr }};
            return impl.run(std::forward<const args_t&>(args)...).wrapped();
        }

    private:


    };
}