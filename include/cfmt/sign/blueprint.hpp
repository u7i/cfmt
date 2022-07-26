// Created by u7i
#pragma once

#include "match.hpp"

namespace cfmt::sign {
    template <typename string> requires Indexable<string>
    struct blueprint {
        using match_type = match<string>;

        blueprint(const string& open, const string& close) noexcept : open_(open), close_(close) {}

        [[nodiscard]] auto extract(const view<string>& str) const noexcept -> match_type
        {
            // Parse string based on open/close parts
            try {
                auto open = indexable::find(str, open_).value();
                auto open_end = open + open_.length();

                auto close = indexable::find(str, close_, open_end).value();
                auto close_end = close + close_.length();

                return { str.shrink(0, open), str.shift(close_end), str.shrink(open_end, close - open_end) };
            }
            catch (const std::exception& exception) {
                return {};
            }
        }

        [[nodiscard]] auto open()  const noexcept -> const string& { return open_; }
        [[nodiscard]] auto close() const noexcept -> const string& { return close_; }

    private:
        string open_;
        string close_;
    };
}