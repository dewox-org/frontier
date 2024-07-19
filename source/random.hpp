#pragma once
#include "type.hpp"

namespace dewox
{
    auto random_identity(Identity* seed) -> Identity;  // [0, 1 << 64).
    auto random_float32(Identity* seed) -> float;      // [0, 1).
    auto random_float64(Identity* seed) -> double;     // [0, 1).
}

