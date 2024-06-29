#include "external/include/itoa/jeaiii_to_text.h"

extern "C"
{
    auto dewox_external_itoa_format_integer_32bit(char buffer[11], int signed a) -> char*
    {
        return jeaiii::to_text_from_integer(buffer, a);
    }

    auto dewox_external_itoa_format_integer_64bit(char buffer[20], int long long signed a) -> char*
    {
        return jeaiii::to_text_from_integer(buffer, a);
    }

    auto dewox_external_itoa_format_unteger_32bit(char buffer[10], int unsigned a) -> char*
    {
        return jeaiii::to_text_from_integer(buffer, a);
    }

    auto dewox_external_itoa_format_unteger_64bit(char buffer[20], int long long unsigned a) -> char*
    {
        return jeaiii::to_text_from_integer(buffer, a);
    }
}

