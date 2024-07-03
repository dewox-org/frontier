#pragma once
#include "type.hpp"

namespace dewox::inline hash
{
    using Hash = Identity;

    auto hash_identity(Identity a) -> Hash;
}

