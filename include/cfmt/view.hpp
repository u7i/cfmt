// Created by u7i
#pragma once

#include "indexable.hpp"

namespace cfmt {
    /** Abstract view for any indexable. */
    template <typename T> requires Indexable<T>
    struct view {
        using string_type = T;
        using size_type = typename T::size_type;

        view(const T& str) noexcept : first_(0), length_(str.length()), str_(&str) {}
        view(size_type first, size_type length, const T& str) noexcept : first_(first), length_(length), str_(&str) {}

        /** Mark a copy of view shifted by n. */
        [[nodiscard]] auto shift(size_type n) const noexcept -> view<T>
        {
            return {first_ + n, length_ - n, *str_};
        }

        /** Shrink view to block that starts at first with given length. */
        [[nodiscard]] auto shrink(size_type first, size_t length) const noexcept -> view<T>
        {
            return view<T>(first_ + first, length, *str_);
        }

        /** Get length of the view. */
        [[nodiscard]] auto length() const noexcept -> size_type
        {
            return length_;
        }

        /** Check if view is empty. */
        [[nodiscard]] auto empty() const noexcept -> bool
        {
            return length_ == 0;
        }

        /** Get element at index i, may throw exceptions ( depending on the string_type implementation ). */
        [[nodiscard]] auto operator[](size_t i) const {
            return (*str_)[first_ + i];
        }

    private:
        size_type first_, length_;
        const T* str_;
    };
}