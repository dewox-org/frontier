#include "test.hpp"
#include "tool.hpp"
#include "site.hpp"
#include "native.hpp"

namespace dewox::inline test
{
    inline namespace
    {
        auto count_digits(Size n) -> int
        {
            if (n == 0) return 1;
            auto count = 0;
            while (n != 0) {
                n /= 10;
                count++;
            }
            return count;
        }

        auto count_test_start(Test* test, char const* name) -> bool
        {
            auto data = (Count_Test*) test->maybe_data;
            data->count++;
            return false;
        }

        auto count_test_finish(Test* test, bool success) -> void
        {
            // Do nothing intentionally.
        }

        auto standard_test_start(Test* test, char const* name) -> bool
        {
            auto data = (Standard_Test*) test->maybe_data;
            data->test_name = name;
            native::printf("[%*zu/%-*zu] Testing %s...\n", data->count_digit_count, data->passed_test_count + data->failed_test_count + 1u, data->count_digit_count, data->count, name);
            native::flush();
            return true;
        }

        auto standard_test_finish(Test* test, bool success) -> void
        {
            auto data = (Standard_Test*) test->maybe_data;
            if (!data->test_name) {
                data->count++;
                data->failed_test_count++;
                data->count_digit_count = count_digits(data->count);
                native::printf("[%*zu/%-*zu] %s %s\n", data->count_digit_count, data->passed_test_count + data->failed_test_count, data->count_digit_count, data->count, "FAIL", "** unpaired finish() **");
                return;
            }
            if (success) {
                data->passed_test_count++;
            } else {
                data->failed_test_count++;
            }
            native::printf("%s[%*zu/%-*zu] %s %s\e[K\n", (success ? "\e[F" : ""), data->count_digit_count, data->passed_test_count + data->failed_test_count, data->count_digit_count, data->count, (success ? " OK " : "FAIL"), data->test_name);
            native::flush();
            data->test_name = {};
        }
    }

    auto Count_Test::into(Count_Test* result) -> void
    {
        result->count = {};
    }

    auto Count_Test::test() -> Test
    {
        return {
            .maybe_data = (void*) this,
            .start = &count_test_start,
            .finish = &count_test_finish,
        };
    }

    auto Standard_Test::into(Standard_Test* result, Size count) -> void
    {
        result->count = count;
        result->count_digit_count = count_digits(count);
        result->passed_test_count = {};
        result->failed_test_count = {};
        result->test_name = {};
    }

    auto Standard_Test::test() -> Test
    {
        return {
            .maybe_data = (void*) this,
            .start = &standard_test_start,
            .finish = &standard_test_finish,
        };
    }

    auto Standard_Test::report() -> void
    {
        native::printf(" %*zu/%-*zu  %s %3d%%\n", count_digit_count, passed_test_count, count_digit_count, count, " OK ", (count > 0u ? int(100 * passed_test_count / count) : 100));
        native::printf(" %*zu/%-*zu  %s %3d%%\n", count_digit_count, failed_test_count, count_digit_count, count, "FAIL", (count > 0u ? int(100 * failed_test_count / count) : 0));
        native::flush();
    }

    auto run_tests_on(Test* test) -> void
    {
        search_sites([] (Site* site, void* data) -> bool {
            auto test = (Test*) data;
            if (site->key.starts_with("test-"_s)) {
                auto run_test = (Run_Test*) site->value;
                run_test(test);
            }
            return false;
        }, (void*) test);
    }

    auto run_tests() -> bool
    {
        auto count = create(&Count_Test::into);
        {
            auto test = count.test();
            run_tests_on(&test);
        }

        auto standard = create(&Standard_Test::into, count.count);
        {
            auto test = standard.test();
            run_tests_on(&test);
        }
        standard.report();

        return (standard.passed_test_count == standard.count);
    }
}

