#include "hash.hpp"

namespace dewox::inline hash
{
    auto hash_identity(Identity a, Identity key0, Identity key1) -> Hash
    {
        // Using SipHash-2-4 from "https://github.com/veorq/SipHash/blob/master/siphash.c".

        constexpr auto c_round_count = 2u;
        constexpr auto d_round_count = 4u;

        auto v0 = Identity(0x736f6d6570736575u) ^ key0;
        auto v1 = Identity(0x646f72616e646f6du) ^ key1;
        auto v2 = Identity(0x6c7967656e657261u) ^ key0;
        auto v3 = Identity(0x7465646279746573u) ^ key1;
        auto b = Identity(Identity(sizeof(Identity)) << 56u);

        auto rotate_left = [] (Identity a, int unsigned offset) -> Identity {
            // [invariant] offset <= 64u;
            return Identity((a << offset) | (a >> (64u - offset)));
        };

        auto sip_rounds = [&] (int unsigned round_count) -> void {
            while (round_count-- > 0u) {
                v0 += v1;
                v1 = rotate_left(v1, 13u);
                v1 ^= v0;
                v0 = rotate_left(v0, 32u);
                v2 += v3;
                v3 = rotate_left(v3, 16u);
                v3 ^= v2;
                v0 += v3;
                v3 = rotate_left(v3, 21u);
                v3 ^= v0;
                v2 += v1;
                v1 = rotate_left(v1, 17u);
                v1 ^= v2;
                v2 = rotate_left(v2, 32u);
            }
        };

        v3 ^= a;
        sip_rounds(c_round_count);
        v0 ^= a;

        v3 ^= b;
        sip_rounds(c_round_count);
        v0 ^= b;

        v2 ^= 0xff;
        sip_rounds(d_round_count);

        return v0 ^ v1 ^ v2 ^ v3;
    }
}

