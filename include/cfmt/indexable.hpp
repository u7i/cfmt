// Created by u7i
#pragma once

#include <optional>
#include <concepts>

namespace cfmt {
    /** Abstract indexable object. */
    template <class T>
    concept Indexable = requires(T a) {
        { a.length() } -> std::same_as<typename T::size_type>;
        a.operator[](0);
    };

    /** Checks if 2 type are compatible as indexable. */
    template <class T, class U>
    concept CompatibleIndexable = Indexable<T> && Indexable<U> && requires(T a, U b) {
        a[0] == b[0];
    };

    namespace indexable {
        /** Find a needle in the string. Both needle & string must be indexable. */
        template <typename T, typename U, typename size_type = typename T::size_type> requires CompatibleIndexable<T, U>
        auto find(const T& str, const U& needle, size_type pos = {}) noexcept -> std::optional<size_type>
        {
            if (needle.length() > str.length() || needle.length() == 0)
                return std::nullopt;

            // Naive find implementation
            const size_type shift = needle.length() - 1;

            for (size_type c = pos, i = 0; c < str.length() - shift; ++c) {
                i = needle[i] == str[c] ? i + 1 : 0;

                if (i == needle.length())
                    return { c - shift };
            }
            return std::nullopt;
        }

        /** Compare two indexable strings. */
        template <typename T, typename U> requires CompatibleIndexable<T, U>
        auto compare(const T& first, const U& second) -> bool
        {
            if (first.length() != second.length())
                return false;

            for (size_t i = 0; i < first.length(); ++i) {
                if (first[i] != second[i])
                    return false;
            }

            return true;
        }
    }
}