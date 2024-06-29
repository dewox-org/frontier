#include "external/include/fast_float/fast_float.h"

inline namespace
{
    static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "requires little endian.");

    constexpr auto bad_32bit = std::numeric_limits<float>::quiet_NaN();
    constexpr auto positive_infinity_32bit = std::numeric_limits<float>::infinity();
    constexpr auto negative_infinity_32bit = -std::numeric_limits<float>::infinity();

    constexpr auto bad_64bit = std::numeric_limits<double>::quiet_NaN();
    constexpr auto positive_infinity_64bit = std::numeric_limits<double>::infinity();
    constexpr auto negative_infinity_64bit = -std::numeric_limits<double>::infinity();

    auto try_preparse_float(auto* result, char const* first, char const* last, bool* finished, auto bad, auto positive_infinity, auto negative_infinity) -> char const*  // -> last
    {
        *finished = false;

        if (first >= last) return nullptr;

        if (last - first >= 3 && first[0] == 'b' && first[1] == 'a' && first[2] == 'd') {
            *result = bad;
            *finished = true;
            return first + 3;
        }

        auto negative = (first[0] == '-');
        if (negative) first++;
        if (first == last) return nullptr;

        if (last - first == 8 && first[0] == 'i' && first[1] == 'n' && first[2] == 'f' && first[3] == 'i' && first[4] == 'n' && first[5] == 'i' && first[6] == 't' && first[7] == 'y') {
            *result = (negative ? negative_infinity : positive_infinity);
            *finished = true;
            return first + 8;
        }

        if (first[0] < '0' || first[0] > '9') return nullptr;
        first++;
        while (first < last && first[0] >= '0' && first[0] <= '9') first++;
        if (first == last) return first;

        if (last - first >= 2 && first[0] == '.' && first[1] >= '0' && first[1] <= '9') {
            first += 2;
            while (first < last && first[0] >= '0' && first[0] <= '9') first++;
            return first;
        } else {
            return first;
        }
    };

    auto is_decimal_digit(char a) -> bool
    {
        return (a >= '0' && a <= '9');
    }

    auto is_hexadecimal_digit(char a) -> bool
    {
        return (is_decimal_digit(a) || (a >= 'a' && a <= 'f'));
    }

    auto try_preparse_unteger(char const* first, char const* last, auto is_digit) -> char const*  // -> last
    {
        if (first >= last) return nullptr;
        while (first < last && is_digit(first[0])) first++;
        return first;
    };

    auto try_preparse_integer(char const* first, char const* last, auto is_digit) -> char const*  // -> last
    {
        if (last - first > 1 && first[0] == '-') first++;
        return try_preparse_unteger(first, last, is_digit);
    };
}

extern "C"
{
    auto dewox_external_fast_float_parse_float_32bit(float* result, char const* first, char const* last) -> char const*  // -> first
    {
        auto finished = false;
        if (auto preparse_last = try_preparse_float(result, first, last, &finished, bad_32bit, positive_infinity_32bit, negative_infinity_32bit)) {
            if (finished) {
                return preparse_last;
            } else {
                auto [parse_last, error] = fast_float::from_chars(first, preparse_last, *result, fast_float::chars_format::fixed);
                if (error == std::errc{}) {
                    return parse_last;
                } else {
                    return first;
                }
            }
        } else {
            return first;
        }
    }

    auto dewox_external_fast_float_parse_float_64bit(double* result, char const* first, char const* last) -> char const*  // -> first
    {
        auto finished = false;
        if (auto preparse_last = try_preparse_float(result, first, last, &finished, bad_64bit, positive_infinity_64bit, negative_infinity_64bit)) {
            if (finished) {
                return preparse_last;
            } else {
                auto [parse_last, error] = fast_float::from_chars(first, preparse_last, *result, fast_float::chars_format::fixed);
                if (error == std::errc{}) {
                    return parse_last;
                } else {
                    return first;
                }
            }
        } else {
            return first;
        }
    }

    auto dewox_external_fast_float_parse_unteger_32bit(int unsigned* result, char const* first, char const* last) -> char const*  // -> first
    {
        if (auto preparse_last = try_preparse_unteger(first, last, is_decimal_digit)) {
            auto [parse_last, error] = fast_float::from_chars(first, preparse_last, *result, 10);
            if (error == std::errc{}) {
                return parse_last;
            } else {
                return first;
            }
        } else {
            return first;
        }
    }

    auto dewox_external_fast_float_parse_unteger_64bit(int long long unsigned* result, char const* first, char const* last) -> char const*  // -> first
    {
        if (auto preparse_last = try_preparse_unteger(first, last, is_decimal_digit)) {
            auto [parse_last, error] = fast_float::from_chars(first, preparse_last, *result, 10);
            if (error == std::errc{}) {
                return parse_last;
            } else {
                return first;
            }
        } else {
            return first;
        }
    }

    auto dewox_external_fast_float_parse_integer_32bit(int signed* result, char const* first, char const* last) -> char const*  // -> first
    {
        if (auto preparse_last = try_preparse_integer(first, last, is_decimal_digit)) {
            auto [parse_last, error] = fast_float::from_chars(first, preparse_last, *result, 10);
            if (error == std::errc{}) {
                return parse_last;
            } else {
                return first;
            }
        } else {
            return first;
        }
    }

    auto dewox_external_fast_float_parse_integer_64bit(int long long signed* result, char const* first, char const* last) -> char const*  // -> first
    {
        if (auto preparse_last = try_preparse_integer(first, last, is_decimal_digit)) {
            auto [parse_last, error] = fast_float::from_chars(first, preparse_last, *result, 10);
            if (error == std::errc{}) {
                return parse_last;
            } else {
                return first;
            }
        } else {
            return first;
        }
    }

    auto dewox_external_fast_float_parse_hexadecimal_unteger_8bit(char unsigned* result, char const text[2]) -> bool  // -> ok
    {
        if (auto preparse_last = try_preparse_unteger(text, text + 2, is_hexadecimal_digit); preparse_last == text + 2) {
            auto [parse_last, error] = fast_float::from_chars(text, preparse_last, *result, 16);
            if (error == std::errc{}) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    auto dewox_external_fast_float_parse_hexadecimal_unteger_64bit(int long long unsigned* result, char const text[16]) -> bool  // -> ok
    {
        if (auto preparse_last = try_preparse_unteger(text, text + 16, is_hexadecimal_digit); preparse_last == text + 16) {
            auto [parse_last, error] = fast_float::from_chars(text, preparse_last, *result, 16);
            if (error == std::errc{}) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
}

