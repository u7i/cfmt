// Created by u7i
#pragma once

#include "bad_match.hpp"
#include "cfmt/view.hpp"

#include <cstring>

namespace cfmt::group {
    template <typename string> requires Indexable<string>
    struct match {
        using size_type = typename string::size_type;
        using view_type = view<string>;
        using const_reference = const view<string>&;

        match() noexcept {
            data_ = std::nullopt;
        }
        match(const_reference prefix, const_reference suffix, const_reference id = {}) noexcept {
            data_ = {
                .prefix = prefix,
                .suffix = suffix,
                .id = id
            };
        }

        [[nodiscard]] auto valid()     const noexcept -> bool { return data_.has_value(); }
        [[nodiscard]] auto anonymous() const          -> bool { return safe_data().id.empty(); }

        [[nodiscard]] auto prefix()    const -> view_type { return safe_data().prefix; }
        [[nodiscard]] auto suffix()    const -> view_type { return safe_data().suffix; }
        [[nodiscard]] auto id()        const -> view_type { return safe_data(!anonymous()).id; }

    private:
        const auto& safe_data(bool additional_cond = true) const {
            if (!data_ || !additional_cond)
                throw bad_match();

            return data_.value();
        }

        struct data_t {
            view_type prefix;
            view_type suffix;
            view_type id;
        };
        std::optional<data_t> data_;
    };
}