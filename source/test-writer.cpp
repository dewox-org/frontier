#include "test.hpp"
#include "tool.hpp"
#include "chain.hpp"
#include "writer.hpp"
#include "object.hpp"

namespace dewox
{
    extern "C" auto dewox_site_test_writer(Test* test) -> void
    {
        if (test->start(test, "writer/i32/0000")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int signed) 0x00000000);
            test->finish(test, (chain->merge(chain->byte_count()) == "0"_s));
        }

        if (test->start(test, "writer/i32/0001")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int signed) 0x00000001);
            test->finish(test, (chain->merge(chain->byte_count()) == "1"_s));
        }

        if (test->start(test, "writer/i32/0100")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int signed) 0x00010000);
            test->finish(test, (chain->merge(chain->byte_count()) == "65536"_s));
        }

        if (test->start(test, "writer/i32/4000")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int signed) 0x40000000);
            test->finish(test, (chain->merge(chain->byte_count()) == "1073741824"_s));
        }

        if (test->start(test, "writer/i32/7fff")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int signed) 0x7fffffff);
            test->finish(test, (chain->merge(chain->byte_count()) == "2147483647"_s));
        }

        if (test->start(test, "writer/i32/8000")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int signed) 0x80000000);
            test->finish(test, (chain->merge(chain->byte_count()) == "-2147483648"_s));
        }

        if (test->start(test, "writer/i32/c000")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int signed) 0xc0000000);
            test->finish(test, (chain->merge(chain->byte_count()) == "-1073741824"_s));
        }

        if (test->start(test, "writer/i32/fe00")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int signed) 0xfffe0000);
            test->finish(test, (chain->merge(chain->byte_count()) == "-131072"_s));
        }

        if (test->start(test, "writer/i32/ffff")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int signed) 0xffffffff);
            test->finish(test, (chain->merge(chain->byte_count()) == "-1"_s));
        }

        if (test->start(test, "writer/u32/0000")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int unsigned) 0x00000000);
            test->finish(test, (chain->merge(chain->byte_count()) == "0"_s));
        }

        if (test->start(test, "writer/u32/0001")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int unsigned) 0x00000001);
            test->finish(test, (chain->merge(chain->byte_count()) == "1"_s));
        }

        if (test->start(test, "writer/u32/0100")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int unsigned) 0x00010000);
            test->finish(test, (chain->merge(chain->byte_count()) == "65536"_s));
        }

        if (test->start(test, "writer/u32/4000")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int unsigned) 0x40000000);
            test->finish(test, (chain->merge(chain->byte_count()) == "1073741824"_s));
        }

        if (test->start(test, "writer/u32/7fff")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int unsigned) 0x7fffffff);
            test->finish(test, (chain->merge(chain->byte_count()) == "2147483647"_s));
        }

        if (test->start(test, "writer/u32/8000")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int unsigned) 0x80000000);
            test->finish(test, (chain->merge(chain->byte_count()) == "2147483648"_s));
        }

        if (test->start(test, "writer/u32/c000")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int unsigned) 0xc0000000);
            test->finish(test, (chain->merge(chain->byte_count()) == "3221225472"_s));
        }

        if (test->start(test, "writer/u32/fe00")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int unsigned) 0xfffe0000);
            test->finish(test, (chain->merge(chain->byte_count()) == "4294836224"_s));
        }

        if (test->start(test, "writer/u32/ffff")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int unsigned) 0xffffffff);
            test->finish(test, (chain->merge(chain->byte_count()) == "4294967295"_s));
        }

        if (test->start(test, "writer/i64/0000")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int long long signed) 0x0000000000000000);
            test->finish(test, (chain->merge(chain->byte_count()) == "0"_s));
        }

        if (test->start(test, "writer/i64/0001")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int long long signed) 0x0000000000000001);
            test->finish(test, (chain->merge(chain->byte_count()) == "1"_s));
        }

        if (test->start(test, "writer/i64/0100")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int long long signed) 0x0000000100000000);
            test->finish(test, (chain->merge(chain->byte_count()) == "4294967296"_s));
        }

        if (test->start(test, "writer/i64/4000")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int long long signed) 0x4000000000000000);
            test->finish(test, (chain->merge(chain->byte_count()) == "4611686018427387904"_s));
        }

        if (test->start(test, "writer/i64/7fff")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int long long signed) 0x7fffffffffffffff);
            test->finish(test, (chain->merge(chain->byte_count()) == "9223372036854775807"_s));
        }

        if (test->start(test, "writer/i64/8000")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int long long signed) 0x8000000000000000);
            test->finish(test, (chain->merge(chain->byte_count()) == "-9223372036854775808"_s));
        }

        if (test->start(test, "writer/i64/c000")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int long long signed) 0xc000000000000000);
            test->finish(test, (chain->merge(chain->byte_count()) == "-4611686018427387904"_s));
        }

        if (test->start(test, "writer/i64/fe00")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int long long signed) 0xfffffffe00000000);
            test->finish(test, (chain->merge(chain->byte_count()) == "-8589934592"_s));
        }

        if (test->start(test, "writer/i64/ffff")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int long long signed) 0xffffffffffffffff);
            test->finish(test, (chain->merge(chain->byte_count()) == "-1"_s));
        }

        if (test->start(test, "writer/u64/0000")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int long long unsigned) 0x0000000000000000);
            test->finish(test, (chain->merge(chain->byte_count()) == "0"_s));
        }

        if (test->start(test, "writer/u64/0001")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int long long unsigned) 0x0000000000000001);
            test->finish(test, (chain->merge(chain->byte_count()) == "1"_s));
        }

        if (test->start(test, "writer/u64/0100")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int long long unsigned) 0x0000000100000000);
            test->finish(test, (chain->merge(chain->byte_count()) == "4294967296"_s));
        }

        if (test->start(test, "writer/u64/4000")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int long long unsigned) 0x4000000000000000);
            test->finish(test, (chain->merge(chain->byte_count()) == "4611686018427387904"_s));
        }

        if (test->start(test, "writer/u64/7fff")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int long long unsigned) 0x7fffffffffffffff);
            test->finish(test, (chain->merge(chain->byte_count()) == "9223372036854775807"_s));
        }

        if (test->start(test, "writer/u64/8000")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int long long unsigned) 0x8000000000000000);
            test->finish(test, (chain->merge(chain->byte_count()) == "9223372036854775808"_s));
        }

        if (test->start(test, "writer/u64/c000")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int long long unsigned) 0xc000000000000000);
            test->finish(test, (chain->merge(chain->byte_count()) == "13835058055282163712"_s));
        }

        if (test->start(test, "writer/u64/fe00")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int long long unsigned) 0xfffffffe00000000);
            test->finish(test, (chain->merge(chain->byte_count()) == "18446744065119617024"_s));
        }

        if (test->start(test, "writer/u64/ffff")) {
            auto chain = Sink<Chain>{};
            write(*chain, (int long long unsigned) 0xffffffffffffffff);
            test->finish(test, (chain->merge(chain->byte_count()) == "18446744073709551615"_s));
        }

        if (test->start(test, "writer/f32/zero/positive")) {
            auto chain = Sink<Chain>{};
            write(*chain, transmute<int unsigned, float>(0x00000000u));
            test->finish(test, (chain->merge(chain->byte_count()) == "0.0"_s));
        }

        if (test->start(test, "writer/f32/zero/negative")) {
            auto chain = Sink<Chain>{};
            write(*chain, transmute<int unsigned, float>(0x80000000u));
            test->finish(test, (chain->merge(chain->byte_count()) == "0.0"_s));
        }

        if (test->start(test, "writer/f32/not-a-number/positive")) {
            auto chain = Sink<Chain>{};
            write(*chain, transmute<int unsigned, float>(0x7fffffffu));
            test->finish(test, (chain->merge(chain->byte_count()) == "bad"_s));
        }

        if (test->start(test, "writer/f32/not-a-number/negative")) {
            auto chain = Sink<Chain>{};
            write(*chain, transmute<int unsigned, float>(0xffffffffu));
            test->finish(test, (chain->merge(chain->byte_count()) == "bad"_s));
        }

        if (test->start(test, "writer/f32/infinity/positive")) {
            auto chain = Sink<Chain>{};
            write(*chain, transmute<int unsigned, float>(0x7f800000u));
            test->finish(test, (chain->merge(chain->byte_count()) == "infinity"_s));
        }

        if (test->start(test, "writer/f32/infinity/negative")) {
            auto chain = Sink<Chain>{};
            write(*chain, transmute<int unsigned, float>(0xff800000u));
            test->finish(test, (chain->merge(chain->byte_count()) == "-infinity"_s));
        }

        if (test->start(test, "writer/f32/max/positive")) {
            auto chain = Sink<Chain>{};
            write(*chain, transmute<int unsigned, float>(0x7f7fffffu));
            test->finish(test, (chain->merge(chain->byte_count()) == "340282350000000000000000000000000000000.0"_s));
        }

        if (test->start(test, "writer/f32/max/negative")) {
            auto chain = Sink<Chain>{};
            write(*chain, transmute<int unsigned, float>(0xff7fffffu));
            test->finish(test, (chain->merge(chain->byte_count()) == "-340282350000000000000000000000000000000.0"_s));
        }

        if (test->start(test, "writer/f32/ulp/positive")) {
            auto chain = Sink<Chain>{};
            write(*chain, transmute<int unsigned, float>(0x00000001u));
            test->finish(test, (chain->merge(chain->byte_count()) == "0.000000000000000000000000000000000000000000001"_s));
        }

        if (test->start(test, "writer/f32/ulp/negative")) {
            auto chain = Sink<Chain>{};
            write(*chain, transmute<int unsigned, float>(0x80000001u));
            test->finish(test, (chain->merge(chain->byte_count()) == "-0.000000000000000000000000000000000000000000001"_s));
        }

        if (test->start(test, "writer/f32/1234.56")) {
            auto chain = Sink<Chain>{};
            write(*chain, 1234.56f);
            test->finish(test, (chain->merge(chain->byte_count()) == "1234.56"_s));
        }

        if (test->start(test, "writer/f32/123400")) {
            auto chain = Sink<Chain>{};
            write(*chain, 123400.0f);
            test->finish(test, (chain->merge(chain->byte_count()) == "123400.0"_s));
        }

        if (test->start(test, "writer/f32/0.001234")) {
            auto chain = Sink<Chain>{};
            write(*chain, 0.001234f);
            test->finish(test, (chain->merge(chain->byte_count()) == "0.001234"_s));
        }

        if (test->start(test, "writer/f32/1234.5678")) {
            auto chain = Sink<Chain>{};
            write(*chain, 1234.5678f);
            test->finish(test, (chain->merge(chain->byte_count()) == "1234.5677"_s));  // Yes, the last digit is "7" not "8" due to the rounding rule.
        }

        if (test->start(test, "writer/f32/1.2e34")) {
            auto chain = Sink<Chain>{};
            write(*chain, 1.2e34f);
            test->finish(test, (chain->merge(chain->byte_count()) == "12000000000000000000000000000000000.0"_s));
        }

        if (test->start(test, "writer/f32/1.2e-34")) {
            auto chain = Sink<Chain>{};
            write(*chain, 1.2e-34f);
            test->finish(test, (chain->merge(chain->byte_count()) == "0.00000000000000000000000000000000012"_s));
        }

        if (test->start(test, "writer/f32/-1.2e34")) {
            auto chain = Sink<Chain>{};
            write(*chain, -1.2e34f);
            test->finish(test, (chain->merge(chain->byte_count()) == "-12000000000000000000000000000000000.0"_s));
        }

        if (test->start(test, "writer/f32/-1.2e-34")) {
            auto chain = Sink<Chain>{};
            write(*chain, -1.2e-34f);
            test->finish(test, (chain->merge(chain->byte_count()) == "-0.00000000000000000000000000000000012"_s));
        }

        if (test->start(test, "writer/f64/zero/positive")) {
            auto chain = Sink<Chain>{};
            write(*chain, transmute<int long long unsigned, double>(0x0000000000000000u));
            test->finish(test, (chain->merge(chain->byte_count()) == "0.0"_s));
        }

        if (test->start(test, "writer/f64/zero/negative")) {
            auto chain = Sink<Chain>{};
            write(*chain, transmute<int long long unsigned, double>(0x8000000000000000u));
            test->finish(test, (chain->merge(chain->byte_count()) == "0.0"_s));
        }

        if (test->start(test, "writer/f64/not-a-number/positive")) {
            auto chain = Sink<Chain>{};
            write(*chain, transmute<int long long unsigned, double>(0x7fffffffffffffffu));
            test->finish(test, (chain->merge(chain->byte_count()) == "bad"_s));
        }

        if (test->start(test, "writer/f64/not-a-number/negative")) {
            auto chain = Sink<Chain>{};
            write(*chain, transmute<int long long unsigned, double>(0xffffffffffffffffu));
            test->finish(test, (chain->merge(chain->byte_count()) == "bad"_s));
        }

        if (test->start(test, "writer/f64/infinity/positive")) {
            auto chain = Sink<Chain>{};
            write(*chain, transmute<int long long unsigned, double>(0x7ff0000000000000u));
            test->finish(test, (chain->merge(chain->byte_count()) == "infinity"_s));
        }

        if (test->start(test, "writer/f64/infinity/negative")) {
            auto chain = Sink<Chain>{};
            write(*chain, transmute<int long long unsigned, double>(0xfff0000000000000u));
            test->finish(test, (chain->merge(chain->byte_count()) == "-infinity"_s));
        }

        if (test->start(test, "writer/f64/max/positive")) {
            auto chain = Sink<Chain>{};
            write(*chain, transmute<int long long unsigned, double>(0x7fefffffffffffffu));
            test->finish(test, (chain->merge(chain->byte_count()) == "179769313486231570000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000.0"_s));
        }

        if (test->start(test, "writer/f64/max/negative")) {
            auto chain = Sink<Chain>{};
            write(*chain, transmute<int long long unsigned, double>(0xffefffffffffffffu));
            test->finish(test, (chain->merge(chain->byte_count()) == "-179769313486231570000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000.0"_s));
        }

        if (test->start(test, "writer/f64/ulp/positive")) {
            auto chain = Sink<Chain>{};
            write(*chain, transmute<int long long unsigned, double>(0x0000000000000001u));
            test->finish(test, (chain->merge(chain->byte_count()) == "0.000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005"_s));
        }

        if (test->start(test, "writer/f64/ulp/negative")) {
            auto chain = Sink<Chain>{};
            write(*chain, transmute<int long long unsigned, double>(0x8000000000000001u));
            test->finish(test, (chain->merge(chain->byte_count()) == "-0.000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005"_s));
        }

        if (test->start(test, "writer/f64/1234.56")) {
            auto chain = Sink<Chain>{};
            write(*chain, 1234.56);
            test->finish(test, (chain->merge(chain->byte_count()) == "1234.56"_s));
        }

        if (test->start(test, "writer/f64/123400")) {
            auto chain = Sink<Chain>{};
            write(*chain, 123400.0);
            test->finish(test, (chain->merge(chain->byte_count()) == "123400.0"_s));
        }

        if (test->start(test, "writer/f64/0.001234")) {
            auto chain = Sink<Chain>{};
            write(*chain, 0.001234);
            test->finish(test, (chain->merge(chain->byte_count()) == "0.001234"_s));
        }

        if (test->start(test, "writer/f64/1234.5678")) {
            auto chain = Sink<Chain>{};
            write(*chain, 1234.5678);
            test->finish(test, (chain->merge(chain->byte_count()) == "1234.5678"_s));
        }

        if (test->start(test, "writer/f64/1.2e34")) {
            auto chain = Sink<Chain>{};
            write(*chain, 1.2e34);
            test->finish(test, (chain->merge(chain->byte_count()) == "12000000000000000000000000000000000.0"_s));
        }

        if (test->start(test, "writer/f64/1.2e-34")) {
            auto chain = Sink<Chain>{};
            write(*chain, 1.2e-34);
            test->finish(test, (chain->merge(chain->byte_count()) == "0.00000000000000000000000000000000012"_s));
        }

        if (test->start(test, "writer/f64/-1.2e34")) {
            auto chain = Sink<Chain>{};
            write(*chain, -1.2e34);
            test->finish(test, (chain->merge(chain->byte_count()) == "-12000000000000000000000000000000000.0"_s));
        }

        if (test->start(test, "writer/f64/-1.2e-34")) {
            auto chain = Sink<Chain>{};
            write(*chain, -1.2e-34);
            test->finish(test, (chain->merge(chain->byte_count()) == "-0.00000000000000000000000000000000012"_s));
        }

        if (test->start(test, "writer/u8/00")) {
            auto chain = Sink<Chain>{};
            write(*chain, (char unsigned) 0x00u);
            test->finish(test, (chain->merge(chain->byte_count()) == "00"_s));
        }

        if (test->start(test, "writer/u8/0c")) {
            auto chain = Sink<Chain>{};
            write(*chain, (char unsigned) 0x0cu);
            test->finish(test, (chain->merge(chain->byte_count()) == "0c"_s));
        }

        if (test->start(test, "writer/u8/7f")) {
            auto chain = Sink<Chain>{};
            write(*chain, (char unsigned) 0x7fu);
            test->finish(test, (chain->merge(chain->byte_count()) == "7f"_s));
        }

        if (test->start(test, "writer/u8/80")) {
            auto chain = Sink<Chain>{};
            write(*chain, (char unsigned) 0x80u);
            test->finish(test, (chain->merge(chain->byte_count()) == "80"_s));
        }

        if (test->start(test, "writer/u8/ff")) {
            auto chain = Sink<Chain>{};
            write(*chain, (char unsigned) 0xffu);
            test->finish(test, (chain->merge(chain->byte_count()) == "ff"_s));
        }

        if (test->start(test, "writer/pointer/nullptr")) {
            auto chain = Sink<Chain>{};
            write(*chain, (void*) nullptr);
            test->finish(test, (chain->merge(chain->byte_count()) == "(00--00-00-00--00)"_s));
        }

        if constexpr (sizeof(void*) == 8u) {
            if (test->start(test, "writer/pointer/12345678abcdef09")) {
                auto chain = Sink<Chain>{};
                write(*chain, (void*) (Size) 0x12345678abcdef09u);
                test->finish(test, (chain->merge(chain->byte_count()) == "(12345678abcdef09)"_s));
            }
        }

        if (test->start(test, "writer/pointer/1234abcd")) {
            auto chain = Sink<Chain>{};
            write(*chain, (void*) (Size) 0x1234abcdu);
            test->finish(test, (chain->merge(chain->byte_count()) == "(000000001234abcd)"_s));
        }
    }
}

