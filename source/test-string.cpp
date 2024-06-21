#include "test.hpp"
#include "string.hpp"
#include "object.hpp"

namespace dewox::inline test
{
    extern "C" auto dewox_site_test_string(Test* test) -> void
    {
        auto pattern = "~AZ@a:a~AZ@@|"_s;
        auto how_pattern1 = "@a|@b:ahello:bworld"_s;
        auto how_pattern2 = "hello|world"_s;

        if (test->start(test, "string/into/literal-size")) {
            test->finish(test, ("hello"_s.byte_count() == 5));
        }

        if (test->start(test, "string/into/until-size")) {
            test->finish(test, (create(&String::until_into, "hello").byte_count() == 5));
        }

        if (test->start(test, "string/into/literal-eq-until")) {
            test->finish(test, ("hello"_s == create(&String::until_into, "hello")));
        }

        if (test->start(test, "string/match")) {
            test->finish(test, ("HElloWORLd"_s.match(pattern) == 2));
        }

        if (test->start(test, "string/search/front")) {
            test->finish(test, ("HElloWORLd"_s.search(pattern) == "HE"_s));
        }

        if (test->start(test, "string/search/middle")) {
            test->finish(test, ("lloWORLd"_s.search(pattern) == "WORL"_s));
        }

        if (test->start(test, "string/search/branching/true")) {
            test->finish(test, ("helworldlo"_s.search(how_pattern1) == "world"_s));
        }

        if (test->start(test, "string/search/branching/fake")) {
            test->finish(test, ("helworldlo"_s.search(how_pattern2) == "helworld"_s));
        }

        if (test->start(test, "string/fill/runtime/result")) {
            char hello_buffer[5]{0,1,2,3,4};
            auto hello = create(&String::item_into<decltype(hello_buffer)>, &hello_buffer);
            test->finish(test, (hello.fill('-') == "-----"_s));
        }

        if (test->start(test, "string/fill/runtime/source")) {
            char hello_buffer[5]{0,1,2,3,4};
            auto hello = create(&String::item_into<decltype(hello_buffer)>, &hello_buffer);
            hello.fill('-');
            test->finish(test, (hello == "-----"_s));
        }

        if (test->start(test, "string/fill/constexpr")) {
            struct Hello
            {
                char buffer[5]{0,1,2,3,4};

                constexpr Hello(int a)
                {
                    auto s = create(&String::size_into, buffer, sizeof(buffer));
                    s.fill(a);
                }
            };
            constexpr auto hello = Hello{'-'};
            auto s = create(&String::item_into<decltype(hello.buffer)>, &hello.buffer);
            test->finish(test, (s == "-----"_s));
        }

        if (test->start(test, "string/copy/runtime/long/result")) {
            char hello_buffer[5]{0,1,2,3,4};
            auto hello = create(&String::item_into<decltype(hello_buffer)>, &hello_buffer);
            test->finish(test, (hello.copy("hello world"_s) == "hello"_s));
        }

        if (test->start(test, "string/copy/runtime/long/source")) {
            char hello_buffer[5]{0,1,2,3,4};
            auto hello = create(&String::item_into<decltype(hello_buffer)>, &hello_buffer);
            hello.copy("hello world"_s);
            test->finish(test, (hello == "hello"_s));
        }

        if (test->start(test, "string/copy/runtime/short/result")) {
            char hello_buffer[5]{0,1,2,3,4};
            auto hello = create(&String::item_into<decltype(hello_buffer)>, &hello_buffer);
            hello.copy("hello world"_s);
            test->finish(test, (hello.copy("wow"_s) == "wow"_s));
        }

        if (test->start(test, "string/copy/runtime/short/source")) {
            char hello_buffer[5]{0,1,2,3,4};
            auto hello = create(&String::item_into<decltype(hello_buffer)>, &hello_buffer);
            hello.copy("hello world"_s);
            hello.copy("wow"_s);
            test->finish(test, (hello == "wowlo"_s));
        }

        if (test->start(test, "string/copy/runtime/prefix")) {
            char hello_buffer[5]{0,1,2,3,4};
            auto hello = create(&String::item_into<decltype(hello_buffer)>, &hello_buffer);
            hello.copy("hello world"_s);
            hello.prefix(2u).copy("world"_s);
            test->finish(test, (hello == "wollo"_s));
        }

        if (test->start(test, "string/copy/constexpr")) {
            struct Hello
            {
                char buffer[5]{0,1,2,3,4};

                constexpr Hello(String source)
                {
                    auto s = create(&String::size_into, buffer, sizeof(buffer));
                    s.copy(source);
                }
            };
            constexpr auto hello = Hello{"hello world"_s};
            auto s = create(&String::item_into<decltype(hello.buffer)>, &hello.buffer);
            test->finish(test, (s == "hello"_s));
        }
    }
}

