/*
 * Copyright 2015, 2016 Grygoriy Fuchedzhy <grygoriy.fuchedzhy@gmail.com>
 */

#ifndef ENGINE_UTILS_HPP
#define ENGINE_UTILS_HPP

#include <glcxx/src/utils.hpp>
#include <random>

/// @brief random generator
extern std::mt19937 random_gen;

/// @brief std::size from C++17
template<typename T>
constexpr auto size(const T& x)
{
   return x.size();
}

/// @brief array overload
template<typename T, size_t N>
constexpr size_t size(const T (&array)[N])
{
   return N;
}

#endif
