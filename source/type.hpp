#pragma once

namespace dewox
{
    using Size = decltype(sizeof(0));               // An unsigned number that has an exact width of a pointer.
    using Identity = decltype(0xefffefffefffefff);  // An unsigned 64-bit number.
}

