#include "writer.hpp"
#include "chain.hpp"
#include "string.hpp"
#include "object.hpp"
#include "native.hpp"

namespace dewox::inline writer
{
    auto write(Chain* chain, char piece) -> void
    {
        *chain->grow(1u).begin() = piece;
    }

    auto write(Chain* chain, char signed piece) -> void
    {
        write(chain, (char unsigned) piece);
    }

    auto write(Chain* chain, char unsigned piece) -> void
    {
        // TODO
    }

    auto write(Chain* chain, int signed piece) -> void
    {
        auto buffer = chain->grow(11u);
        auto n = buffer.end() - native::format_integer_32bit(buffer.begin(), piece);
        chain->pop((Size) n);
    }

    auto write(Chain* chain, int unsigned piece) -> void
    {
        auto buffer = chain->grow(10u);
        auto n = buffer.end() - native::format_unteger_32bit(buffer.begin(), piece);
        chain->pop((Size) n);
    }

    auto write(Chain* chain, int short signed piece) -> void
    {
        write(chain, (int signed) piece);
    }

    auto write(Chain* chain, int short unsigned piece) -> void
    {
        write(chain, (int unsigned) piece);
    }

    auto write(Chain* chain, int long signed piece) -> void
    {
        constexpr auto probe = (int long long unsigned) -1LL;
        if constexpr ((int long long unsigned) (int long unsigned) probe == probe) {
            write(chain, (int long long signed) piece);
        } else {
            write(chain, (int signed) piece);
        }
    }

    auto write(Chain* chain, int long unsigned piece) -> void
    {
        constexpr auto probe = (int long long unsigned) -1LL;
        if constexpr ((int long long unsigned) (int long unsigned) probe == probe) {
            write(chain, (int long long unsigned) piece);
        } else {
            write(chain, (int unsigned) piece);
        }
    }

    auto write(Chain* chain, int long long signed piece) -> void
    {
        auto buffer = chain->grow(20u);
        auto n = buffer.end() - native::format_integer_64bit(buffer.begin(), piece);
        chain->pop((Size) n);
    }

    auto write(Chain* chain, int long long unsigned piece) -> void
    {
        auto buffer = chain->grow(20u);
        auto n = buffer.end() - native::format_unteger_64bit(buffer.begin(), piece);
        chain->pop((Size) n);
    }

    auto write(Chain* chain, float piece) -> void
    {
        // TODO
    }

    auto write(Chain* chain, double piece) -> void
    {
        // TODO
    }

    auto write(Chain* chain, void* piece) -> void
    {
        // TODO
    }

    auto write(Chain* chain, char* piece) -> void
    {
        write(chain, (void*) piece);
    }

    auto write(Chain* chain, char const* piece) -> void
    {
        write(chain, create(&String::until_into, piece));
    }
}

