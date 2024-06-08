#include "site.hpp"

namespace dewox::inline site
{
    auto search_sites(Find_Site* find, void* maybe_data) -> bool
    {
        auto site = dewox_sites;
        while (site->value) {
            if (find(site, maybe_data)) {
                return true;
            }
            site++;
        }
        return false;
    }
}

