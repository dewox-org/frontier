#include "test.hpp"
#include "chain.hpp"
#include "writer.hpp"
#include "object.hpp"

namespace dewox::inline test
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
    }
}

