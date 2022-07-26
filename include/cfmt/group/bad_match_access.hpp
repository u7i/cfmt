// Created by u7i
#pragma once

#include <exception>

namespace cfmt::group {
    struct bad_match_access : public std::exception {
        [[nodiscard]] auto what() const noexcept -> const char* final
        {
            return "Match is invalid.";
        }
    };
}