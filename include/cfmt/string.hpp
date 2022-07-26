// Created by u7i.
#pragma once

#include "indexable.hpp"
#include "view.hpp"

namespace cfmt {
    /** Abstract string object. */
    template <typename T>
    concept String = Indexable<T> && requires(T a) {
        T {};
        T { std::declval<typename T::value_type>() };

        a.append_block(std::declval<view<T>>());
        a.append_value(0);
        a.wrapped();
    };
}