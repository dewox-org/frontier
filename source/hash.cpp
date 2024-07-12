#include "hash.hpp"

namespace dewox::inline hash
{
    auto hash_identity(Identity a, Identity seed) -> Identity
    {
        // Reduced from MurmurHash64A.

        auto m = Identity(0xc6a4a7935bd1e995u);
        auto r = 47u;

        auto h = seed ^ Identity(8u * m);

        a *= m;
        a ^= a >> r;
        a *= m;

        h ^= a;
        h *= m;

        h ^= h >> r;
        h *= m;
        h ^= h >> r;

        return h;
    }
}

