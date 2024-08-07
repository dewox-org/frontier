#include "type.hpp"

namespace dewox
{
    inline namespace
    {
        static_assert(sizeof(Size) >= 4u);
        static_assert(sizeof(Size) <= sizeof(Identity));
        static_assert(sizeof(Size) == alignof(Size));
        static_assert(Size(-1) > Size(0));

        static_assert(sizeof(Identity) == 8u);
        static_assert(sizeof(Identity) == alignof(Identity));
        static_assert(Identity(-1) > Identity(0));

        static_assert(sizeof(int short signed) == 2u);
        static_assert(sizeof(int signed) == 4u);
        static_assert(sizeof(int long long signed) == 8u);
    }
}

