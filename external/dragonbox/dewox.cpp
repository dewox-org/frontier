#include "external/include/dragonbox/dragonbox.h"

inline namespace
{
    auto format_float(char* buffer, auto a, auto format_unteger) -> char*
    {
        if (a != a) {
            *buffer++ = 'b';
            *buffer++ = 'a';
            *buffer++ = 'd';
            return buffer;
        }

        if (a == 0) {
            *buffer++ = '0';
            *buffer++ = '.';
            *buffer++ = '0';
            return buffer;
        }

        if (a < 0) {
            *buffer++ = '-';
            a = -a;
        }

        if (a - a != a - a) {
            *buffer++ = 'i';
            *buffer++ = 'n';
            *buffer++ = 'f';
            *buffer++ = 'i';
            *buffer++ = 'n';
            *buffer++ = 'i';
            *buffer++ = 't';
            *buffer++ = 'y';
            return buffer;
        }

        auto [significand, exponent] = jkj::dragonbox::to_decimal(a, jkj::dragonbox::policy::sign::ignore, jkj::dragonbox::policy::cache::compact);

        if (exponent >= 0) {
            buffer = format_unteger(buffer, significand);
            while (exponent-- > 0) *buffer++ = '0';
            *buffer++ = '.';
            *buffer++ = '0';
            return buffer;
        } else {
            char digits[20];
            int digit_count = int(format_unteger(digits, significand) - digits);
            if (-exponent >= digit_count) {
                *buffer++ = '0';
                *buffer++ = '.';
                while (exponent++ < -digit_count) *buffer++ = '0';
                auto digit = digits;
                while (digit_count-- > 0) *buffer++ = *digit++;

            } else {
                auto dot = digit_count + exponent;
                auto digit = digits;
                while (dot-- > 0) *buffer++ = *digit++;
                *buffer++ = '.';
                while (exponent++ < 0) *buffer++ = *digit++;
            }
            return buffer;
        }
    }
}

extern "C"
{
    auto dewox_external_itoa_format_unteger_32bit(char buffer[10], int unsigned a) -> char*;
    auto dewox_external_itoa_format_unteger_64bit(char buffer[20], int long long unsigned a) -> char*;

    // buffer-capacioty = sign(1) + zero(1) + decimal-point(1) + significand(9) + exponent(38)
    auto dewox_external_dragonbox_format_float_32bit(char buffer[50], float a) -> char*
    {
        return format_float(buffer, a, &dewox_external_itoa_format_unteger_32bit);
    }

    // buffer-capacioty = sign(1) + zero(1) + decimal-point(1) + significand(17) + exponent(308)
    auto dewox_external_dragonbox_format_float_64bit(char buffer[328], double a) -> char*
    {
        return format_float(buffer, a, &dewox_external_itoa_format_unteger_64bit);
    }
}

