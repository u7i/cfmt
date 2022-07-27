// Created by u7i
#pragma once

#include "cfmt/view.hpp"

#include <functional>
#include <vector>

namespace cfmt::group {
    template <typename string> requires Indexable<string>
    struct cache {
        using view_type = view<string>;
        using const_reference = const view_type&;

        void push(const_reference id, const_reference data) {
            data_.emplace_back(id, data);
        }

        auto find(const_reference id) const noexcept -> std::optional<view_type>
        {
            for (const auto& [gid, gdata] : data_) {
                if (indexable::find(gid, id))
                    return gdata;
            }
            return std::nullopt;
        }

    private:
        std::vector<std::pair<view_type, view_type>> data_;
    };
}