#include "tool.hpp"
#include "object.hpp"
#include "string.hpp"
#include <cstdio>

namespace dewox::inline tool
{
    inline namespace
    {
        struct Trace_Filter final
        {
            Traceable* traceable;
            void* opt_data;

            static constexpr auto into(Trace_Filter* result, Traceable* traceable, void* opt_data) -> void
            {
                result->traceable = traceable;
                result->opt_data = opt_data;
            }
        };

        thread_local auto trace_filter = create(&Trace_Filter::into, &constant_traceable, (void*) true);
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
        if (!trace_filter.traceable(trace_filter.opt_data, opt_message, file, line, function)) return;
        auto line_digit_count = count_digits(line);
        if (line_alignment < line_digit_count) line_alignment = line_digit_count;
        if (auto message = opt_message; message && message[0]) {
            std::fprintf(stderr, "%s:%-*d (%s): %s\n", file, line_alignment, line, function, message);
        } else {
            std::fprintf(stderr, "%s:%-*d (%s)\n", file, line_alignment, line, function);
        }
        std::fflush(stderr);
    }

    auto filter_trace(Traceable* opt_traceable, void* opt_data) -> void
    {
        if (auto traceable = opt_traceable) {
            trace_filter = create(&Trace_Filter::into, traceable, opt_data);
        } else {
            trace_filter = create(&Trace_Filter::into, &constant_traceable, opt_data);
        }
    }

    auto constant_traceable(void* constant, char const* opt_message, char const* file, int line, char const* function) -> bool
    {
        return (bool) constant;
    }

    auto some_file_traceable(void* filename, char const* opt_message, char const* file, int line, char const* function) -> bool
    {
        return (create(&String::until_into, (char const*) filename) == create(&String::until_into, file));
    }
}
