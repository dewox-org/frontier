#pragma once
#include <stddef.h>
#include <stdint.h>
#include <string.h>

namespace std
{
    #define assert(...)

    using ::size_t;
    using ::int32_t;
    using ::uint32_t;
    using ::uint64_t;
    using ::memcpy;

    template <typename T>
    inline constexpr auto is_unsigned_v = (T(-1) > T{});

    template <typename T> concept unsigned_type = is_unsigned_v<T>;

    template <typename T>
    struct numeric_limits
    {
        inline static constexpr auto is_iec559 = true;
        inline static constexpr auto radix = 2;
    };

    template <unsigned_type T>
    struct numeric_limits<T>
    {
        inline static constexpr auto is_iec559 = false;
        inline static constexpr auto radix = 2;
        inline static constexpr auto digits = 8u * sizeof(T);
        inline static constexpr auto max() -> T { return (T) -1; }
    };

    template <bool Pred, typename True, typename False> struct conditional;
    template <typename True, typename False> struct conditional<true, True, False> { using type = True; };
    template <typename True, typename False> struct conditional<false, True, False> { using type = False; };
    template <bool Pred, typename True, typename False> using conditional_t = typename conditional<Pred, True, False>::type;

    template <bool Pred, typename True = void> struct enable_if {};
    template <typename True> struct enable_if<true, True> { using type = True; };
    template <bool Pred, typename True = void> using enable_if_t = typename enable_if<Pred, True>::type;

    template <typename T, typename U> inline constexpr auto is_same_v = false;
    template <typename T> inline constexpr auto is_same_v<T, T> = true;

    template <typename Base, typename T> inline constexpr auto is_base_of_v = !is_same_v<Base, T> && requires (T a) { a.~Base(); };
}

