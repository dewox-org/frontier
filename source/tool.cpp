#include "tool.hpp"
#include <cstdio>

namespace dewox::inline tool
{
    inline namespace
    {
        thread_local auto line_alignment = 0;
        auto count_digits(int n) -> int
        {
            if (n == 0) return 1;
            auto count = 0;
            while (n != 0) {
                n /= 10;
                count++;
            }
            return count;
        }
    }

    auto trace(char const* opt_message, char const* file, int line, char const* function) -> void
    {
        auto line_digit_count = count_digits(line);
        if (line_alignment < line_digit_count) line_alignment = line_digit_count;
        if (auto message = opt_message; message && message[0]) {
            std::fprintf(stderr, "%s:%-*d (%s): %s\n", file, line_alignment, line, function, message);
        } else {
            std::fprintf(stderr, "%s:%-*d (%s)\n", file, line_alignment, line, function);
        }
        std::fflush(stderr);
    }
}

