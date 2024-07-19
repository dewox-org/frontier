#pragma once
#include "type.hpp"

namespace dewox
{
    using Hash = Identity;

    auto hash_identity(Identity a, Identity seed = {}) -> Hash;
}

