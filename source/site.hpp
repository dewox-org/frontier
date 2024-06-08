#pragma once
#include "string.hpp"

namespace dewox::inline site
{
    struct Site final
    {
        String key;
        void* value;

        static constexpr auto into(Site* result, String key, void* value) -> void;
    };

    using Find_Site = auto (Site* site, void* maybe_data) -> bool;

    auto search_sites(Find_Site* find, void* maybe_data = {}) -> bool;

    extern "C" Site dewox_sites[];
}

namespace dewox::inline site
{
    inline constexpr auto Site::into(Site* result, String key, void* value) -> void
    {
        result->key = key;
        result->value = value;
    }
}

