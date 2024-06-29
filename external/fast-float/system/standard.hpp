#pragma once
#include <float.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

namespace std
{
    #define assert(...)

    using ::size_t;
    using ::ptrdiff_t;
    using ::int32_t;
    using ::int64_t;
    using ::uint64_t;
    using ::memcpy;

    enum errc
    {
        ok,
        invalid_argument,
        result_out_of_range,
    };

    template <typename T> struct is_signed { inline static constexpr auto value = (T(-1) < T{}); };
    template <typename T> concept signed_type = is_signed<T>::value;
    template <typename T> concept unsigned_type = !is_signed<T>::value;

    template <typename T>
    struct numeric_limits;

    template <signed_type T>
    struct numeric_limits<T>
    {
        inline static constexpr auto max() -> T { return T(~(T(-1) << (sizeof(T) * 8u - 1u))); }
    };

    template <unsigned_type T>
    struct numeric_limits<T>
    {
        inline static constexpr auto max() -> T { return T(-1); }
    };

    template <>
    struct numeric_limits<float>
    {
        inline static constexpr auto min() -> float { return __builtin_bit_cast(float, (int unsigned) 0x00000001u); }
        inline static constexpr auto quiet_NaN() -> float { return __builtin_bit_cast(float, (int unsigned) 0x7fffffffu); }
        inline static constexpr auto infinity() -> float { return __builtin_bit_cast(float, (int unsigned) 0x7f800000u); }
    };

    template <>
    struct numeric_limits<double>
    {
        inline static constexpr auto min() -> double { return __builtin_bit_cast(double, (int long long unsigned) 0x0000000000000001u); }
        inline static constexpr auto quiet_NaN() -> double { return __builtin_bit_cast(double, (int long long unsigned) 0x7fffffffffffffffu); }
        inline static constexpr auto infinity() -> double { return __builtin_bit_cast(double, (int long long unsigned) 0x7ff0000000000000u); }
    };

    template <typename T, typename U> struct is_same { inline static constexpr auto value = false; };
    template <typename T> struct is_same<T, T> { inline static constexpr auto value = true; };

    template <bool Pred, typename True, typename False> struct conditional;
    template <typename True, typename False> struct conditional<true, True, False> { using type = True; };
    template <typename True, typename False> struct conditional<false, True, False> { using type = False; };

    template <bool Pred, typename True = void> struct enable_if {};
    template <typename True> struct enable_if<true, True> { using type = True; };

    inline constexpr auto distance(auto first, auto last) -> ptrdiff_t { return last - first; }

    inline constexpr auto copy_n(auto data, size_t size, auto target) -> decltype(target)
    {
        while (size-- > 0u) *target++ = *data++;
        return target;
    }

    inline constexpr auto copy_backward(auto first, auto last, auto target_last) -> decltype(target_last)
    {
        while (first < last) *--target_last = *--last;
        return target_last;
    }

    inline constexpr auto fill(auto first, auto last, auto value) -> void
    {
        while (first < last) *first++ = value;
    }

    inline constexpr auto min(auto const& a, decltype(a) b) -> decltype(a) { return (a < b ? a : b); }
}

