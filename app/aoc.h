#ifndef AOC_AOC_H
#define AOC_AOC_H

// Common includes for Advent of Code solutions
#include "aoc/core.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <span>
#include <string>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#define SOLVE extern "C" void solve([[maybe_unused]] int argc, [[maybe_unused]] char **argv)

#if AOC_LOG_ENABLED
#define AOC_LOG(msg) std::clog << msg << std::endl
#else
#define AOC_LOG(msg)                                                                                                       \
    do                                                                                                                 \
    {                                                                                                                  \
    } while (0)
#endif

#endif
