#include "random.hpp"
#include "hash.hpp"

namespace dewox
{
    inline namespace
    {
        union Unfloat32
        {
            int unsigned u;
            float f;
        };

        union Unfloat64
        {
            Identity u;
            double f;
        };

        static_assert(sizeof(Unfloat32::u) == sizeof(Unfloat32::f));
        static_assert(sizeof(Unfloat64::u) == sizeof(Unfloat64::f));
    }

    auto random_identity(Identity* seed) -> Identity
    {
        return (*seed = hash_identity(*seed));
    }

    auto random_float32(Identity* seed) -> float
    {
        auto uf = Unfloat32{.u = ((int unsigned) random_identity(seed) & 0x007fffffu) | 0x3f800000u};
        return uf.f - 1.0f;
    }

    auto random_float64(Identity* seed) -> double
    {
        auto uf = Unfloat64{.u = (random_identity(seed) & 0x000fffffffffffffu) | 0x3ff0000000000000u};
        return uf.f - 1.0;
    }
}

