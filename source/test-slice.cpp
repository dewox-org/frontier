#include "test.hpp"
#include "slice.hpp"
#include "object.hpp"

namespace dewox
{
    extern "C" auto dewox_site_test_slice(Test* test) -> void
    {
        if (test->start(test, "slice/count/byte")) {
            Identity identity[4]{1, 2, 3, 4};
            auto s = create(&Slice<Identity>::count_into, identity, 4u);
            test->finish(test, (s.byte_count() == 32u));
        }

        if (test->start(test, "slice/count/item")) {
            Identity identity[4]{1, 2, 3, 4};
            auto s = create(&Slice<Identity>::count_into, identity, 4u);
            test->finish(test, (s.item_count() == 4));
        }

        if (test->start(test, "slice/count/iterator")) {
            Identity identity[4]{1, 2, 3, 4};
            auto s = create(&Slice<Identity>::count_into, identity, 4u);
            test->finish(test, (s.end() - s.begin() == 4));
        }
    }
}

