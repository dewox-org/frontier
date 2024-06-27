#include "test.hpp"
#include "tool.hpp"
#include "hash.hpp"
#include "chain.hpp"
#include "journal.hpp"
#include "native.hpp"

namespace dewox::inline test
{
    extern "C" auto dewox_site_test_number_parsing(Test* test) -> void
    {
        auto chain = Sink<Chain>{};
        auto journal_chain = *chain;
        use_journal(&journal_chain);

        if (test->start(test, "number-parsing/f32/zero/positive")) {
            auto a = transmute<int unsigned, float>(0x00000000u);
            auto b = format(a);
            auto c = decltype(a){};
            auto d = native::parse_float_32bit(&c, b.begin(), b.end());
            test->finish(test, (a == c && b.byte_count() == d));
        }

        if (test->start(test, "number-parsing/f32/zero/negative")) {
            auto a = transmute<int unsigned, float>(0x80000000u);
            auto b = format(a);
            auto c = decltype(a){};
            auto d = native::parse_float_32bit(&c, b.begin(), b.end());
            test->finish(test, (a == c && b.byte_count() == d));
        }

        if (test->start(test, "number-parsing/f32/not-a-number/positive")) {
            auto a = transmute<int unsigned, float>(0x7fffffffu);
            auto b = format(a);
            auto c = decltype(a){};
            auto d = native::parse_float_32bit(&c, b.begin(), b.end());
            test->finish(test, (c != c && b.byte_count() == d));
        }

        if (test->start(test, "number-parsing/f32/not-a-number/negative")) {
            auto a = transmute<int unsigned, float>(0xffffffffu);
            auto b = format(a);
            auto c = decltype(a){};
            auto d = native::parse_float_32bit(&c, b.begin(), b.end());
            test->finish(test, (c != c && b.byte_count() == d));
        }

        if (test->start(test, "number-parsing/f32/infinity/positive")) {
            auto a = transmute<int unsigned, float>(0x7f800000u);
            auto b = format(a);
            auto c = decltype(a){};
            auto d = native::parse_float_32bit(&c, b.begin(), b.end());
            test->finish(test, (a == c && b.byte_count() == d));
        }

        if (test->start(test, "number-parsing/f32/infinity/negative")) {
            auto a = transmute<int unsigned, float>(0xff800000u);
            auto b = format(a);
            auto c = decltype(a){};
            auto d = native::parse_float_32bit(&c, b.begin(), b.end());
            test->finish(test, (a == c && b.byte_count() == d));
        }

        if (test->start(test, "number-parsing/f32/round-trip/positive")) {
            auto failure_count = Size{};
            auto sample_count = Size{};
            for (auto i = (int unsigned) 0x00000000u; i < (int unsigned) 0x7f800000u; i += 0x00010000u) {
                chain->clear();
                auto a = transmute<int unsigned, float>(i | (0x0000ffffu & (int unsigned) hash_identity((Identity) i)));
                auto b = format(a);
                auto c = decltype(a){};
                auto d = native::parse_float_32bit(&c, b.begin(), b.end());
                if (b.byte_count() != d || a != c) {
                    print("Round-trip failed for f32 ", a, " != ", c);
                    failure_count++;
                }
                sample_count++;
            }
            if (failure_count > 0u) {
                print("Failed ", failure_count, "/", sample_count, " f32 round-trips.");
            }
            test->finish(test, (failure_count == 0u));
        }

        if (test->start(test, "number-parsing/f32/round-trip/negative")) {
            auto failure_count = Size{};
            auto sample_count = Size{};
            for (auto i = (int unsigned) 0x80000000u; i < (int unsigned) 0xff800000u; i += 0x00010000u) {
                chain->clear();
                auto a = transmute<int unsigned, float>(i | (0x0000ffffu & (int unsigned) hash_identity((Identity) i)));
                auto b = format(a);
                auto c = decltype(a){};
                auto d = native::parse_float_32bit(&c, b.begin(), b.end());
                if (b.byte_count() != d || a != c) {
                    print("Round-trip failed for f32 ", a, " != ", c);
                    failure_count++;
                }
                sample_count++;
            }
            if (failure_count > 0u) {
                print("Failed ", failure_count, "/", sample_count, " f32 round-trips.");
            }
            test->finish(test, (failure_count == 0u));
        }

        if (test->start(test, "number-parsing/f64/zero/positive")) {
            auto a = transmute<int long long unsigned, double>(0x0000000000000000u);
            auto b = format(a);
            auto c = decltype(a){};
            auto d = native::parse_float_64bit(&c, b.begin(), b.end());
            test->finish(test, (a == c && b.byte_count() == d));
        }

        if (test->start(test, "number-parsing/f64/zero/negative")) {
            auto a = transmute<int long long unsigned, double>(0x8000000000000000u);
            auto b = format(a);
            auto c = decltype(a){};
            auto d = native::parse_float_64bit(&c, b.begin(), b.end());
            test->finish(test, (a == c && b.byte_count() == d));
        }

        if (test->start(test, "number-parsing/f64/not-a-number/positive")) {
            auto a = transmute<int long long unsigned, double>(0x7fffffffffffffffu);
            auto b = format(a);
            auto c = decltype(a){};
            auto d = native::parse_float_64bit(&c, b.begin(), b.end());
            test->finish(test, (c != c && b.byte_count() == d));
        }

        if (test->start(test, "number-parsing/f64/not-a-number/negative")) {
            auto a = transmute<int long long unsigned, double>(0xffffffffffffffffu);
            auto b = format(a);
            auto c = decltype(a){};
            auto d = native::parse_float_64bit(&c, b.begin(), b.end());
            test->finish(test, (c != c && b.byte_count() == d));
        }

        if (test->start(test, "number-parsing/f64/infinity/positive")) {
            auto a = transmute<int long long unsigned, double>(0x7ff0000000000000u);
            auto b = format(a);
            auto c = decltype(a){};
            auto d = native::parse_float_64bit(&c, b.begin(), b.end());
            test->finish(test, (a == c && b.byte_count() == d));
        }

        if (test->start(test, "number-parsing/f64/infinity/negative")) {
            auto a = transmute<int long long unsigned, double>(0xfff0000000000000u);
            auto b = format(a);
            auto c = decltype(a){};
            auto d = native::parse_float_64bit(&c, b.begin(), b.end());
            test->finish(test, (a == c && b.byte_count() == d));
        }

        if (test->start(test, "number-parsing/f64/round-trip/positive")) {
            auto failure_count = Size{};
            auto sample_count = Size{};
            for (auto i = (int long long unsigned) 0x0000000000000000u; i < (int long long unsigned) 0x7ff0000000000000u; i += 0x0001000000000000u) {
                chain->clear();
                auto a = transmute<int long long unsigned, double>(i | (0x0000ffffffffffffu & (int long long unsigned) hash_identity((Identity) i)));
                auto b = format(a);
                auto c = decltype(a){};
                auto d = native::parse_float_64bit(&c, b.begin(), b.end());
                if (b.byte_count() != d || a != c) {
                    print("Round-trip failed for f64 ", a, " != ", c);
                    failure_count++;
                }
                sample_count++;
            }
            if (failure_count > 0u) {
                print("Failed ", failure_count, "/", sample_count, " f64 round-trips.");
            }
            test->finish(test, (failure_count == 0u));
        }

        if (test->start(test, "number-parsing/f64/round-trip/negative")) {
            auto failure_count = Size{};
            auto sample_count = Size{};
            for (auto i = (int long long unsigned) 0x8000000000000000u; i < (int long long unsigned) 0xfff0000000000000u; i += 0x0001000000000000u) {
                chain->clear();
                auto a = transmute<int long long unsigned, double>(i | (0x0000ffffffffffffu & (int long long unsigned) hash_identity((Identity) i)));
                auto b = format(a);
                auto c = decltype(a){};
                auto d = native::parse_float_64bit(&c, b.begin(), b.end());
                if (b.byte_count() != d || a != c) {
                    print("Round-trip failed for f64 ", a, " != ", c);
                    failure_count++;
                }
                sample_count++;
            }
            if (failure_count > 0u) {
                print("Failed ", failure_count, "/", sample_count, " f64 round-trips.");
            }
            test->finish(test, (failure_count == 0u));
        }

        if (test->start(test, "number-parsing/i32/leading-zero-decimal")) {
            auto a = (int signed) {};
            auto b = "010"_s;
            auto c = native::parse_integer_32bit(&a, b.begin(), b.end());
            test->finish(test, (a == 10 && b.byte_count() == c));
        }

        if (test->start(test, "number-parsing/i32/round-trip/positive")) {
            auto a = (int signed) 0x7f7f7f7fu;
            auto b = format(a);
            auto c = decltype(a){};
            auto d = native::parse_integer_32bit(&c, b.begin(), b.end());
            test->finish(test, (a == c && b.byte_count() == d));
        }

        if (test->start(test, "number-parsing/i32/round-trip/negative")) {
            auto a = (int signed) 0xefefefefu;
            auto b = format(a);
            auto c = decltype(a){};
            auto d = native::parse_integer_32bit(&c, b.begin(), b.end());
            test->finish(test, (a == c && b.byte_count() == d));
        }

        if (test->start(test, "number-parsing/u32/round-trip/positive")) {
            auto a = (int unsigned) 0x7f7f7f7fu;
            auto b = format(a);
            auto c = decltype(a){};
            auto d = native::parse_unteger_32bit(&c, b.begin(), b.end());
            test->finish(test, (a == c && b.byte_count() == d));
        }

        if (test->start(test, "number-parsing/u32/round-trip/negative")) {
            auto a = (int unsigned) 0xefefefefu;
            auto b = format(a);
            auto c = decltype(a){};
            auto d = native::parse_unteger_32bit(&c, b.begin(), b.end());
            test->finish(test, (a == c && b.byte_count() == d));
        }

        if (test->start(test, "number-parsing/i64/round-trip/positive")) {
            auto a = (int long long signed) 0x7fff7fff7fff7fffu;
            auto b = format(a);
            auto c = decltype(a){};
            auto d = native::parse_integer_64bit(&c, b.begin(), b.end());
            test->finish(test, (a == c && b.byte_count() == d));
        }

        if (test->start(test, "number-parsing/i64/round-trip/negative")) {
            auto a = (int long long signed) 0xefffefffefffefffu;
            auto b = format(a);
            auto c = decltype(a){};
            auto d = native::parse_integer_64bit(&c, b.begin(), b.end());
            test->finish(test, (a == c && b.byte_count() == d));
        }

        if (test->start(test, "number-parsing/u64/round-trip/positive")) {
            auto a = (int long long unsigned) 0x7fff7fff7fff7fffu;
            auto b = format(a);
            auto c = decltype(a){};
            auto d = native::parse_unteger_64bit(&c, b.begin(), b.end());
            test->finish(test, (a == c && b.byte_count() == d));
        }

        if (test->start(test, "number-parsing/u64/round-trip/negative")) {
            auto a = (int long long unsigned) 0xefffefffefffefffu;
            auto b = format(a);
            auto c = decltype(a){};
            auto d = native::parse_unteger_64bit(&c, b.begin(), b.end());
            test->finish(test, (a == c && b.byte_count() == d));
        }

        if (test->start(test, "number-parsing/hex8")) {
            auto a = (char unsigned) {};
            auto b = "e4"_s;
            auto c = native::parse_hexadecimal_unteger_8bit(&a, b.begin());
            test->finish(test, (a == (char unsigned) '\xe4' && c));
        }

        if (test->start(test, "number-parsing/hex64")) {
            auto a = (int long long unsigned) {};
            auto b = "12345678abcdef09"_s;
            auto c = native::parse_hexadecimal_unteger_64bit(&a, b.begin());
            test->finish(test, (a == (int long long unsigned) 0x12345678abcdef09 && c));
        }

        use_journal(&journal_chain);
    }
}

