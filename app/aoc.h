#pragma once

#include <cstdint>

#define SOLVE extern "C" void solve([[maybe_unused]] int argc, [[maybe_unused]] char** argv)

using S64 = std::int64_t;
using S32 = std::int32_t;
using S16 = std::int16_t;
using S8 = std::int8_t;
using U64 = std::uint64_t;
using U32 = std::uint32_t;
using U16 = std::uint16_t;
using U8 = std::uint8_t;
