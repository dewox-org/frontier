#pragma once
#include "type.hpp"

namespace dewox::inline test
{
    extern "C"
    struct Test
    {
        void* maybe_data;
        auto (*start)(Test* test, char const* name) -> bool;
        auto (*finish)(Test* test, bool success) -> void;
    };

    using Run_Test = auto (Test* test) -> void;

    struct Count_Test
    {
        Size count;

        static auto into(Count_Test* result) -> void;

        auto test() -> Test;
    };

    struct Standard_Test
    {
        Size count;
        Size passed_test_count;
        Size failed_test_count;
        char const* test_name;
        int count_digit_count;

        static auto into(Standard_Test* result, Size count) -> void;

        auto test() -> Test;
        auto report() -> void;
    };

    auto run_tests_on(Test* test) -> void;
    auto run_tests() -> bool;
}

