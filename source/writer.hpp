#pragma once

namespace dewox
{
    struct Chain;
}

namespace dewox
{
    // Overload the "write()" function and write a piece into the chain for custom journal writes.
    // You must use the alignment of 0 (which is the default value).

    // Text characters.
    auto write(Chain* chain, char piece) -> void;

    // Bytes.
    auto write(Chain* chain, char signed piece) -> void;
    auto write(Chain* chain, char unsigned piece) -> void;

    // Integer numbers.
    auto write(Chain* chain, int signed piece) -> void;
    auto write(Chain* chain, int unsigned piece) -> void;
    auto write(Chain* chain, int short signed piece) -> void;
    auto write(Chain* chain, int short unsigned piece) -> void;
    auto write(Chain* chain, int long signed piece) -> void;
    auto write(Chain* chain, int long unsigned piece) -> void;
    auto write(Chain* chain, int long long signed piece) -> void;
    auto write(Chain* chain, int long long unsigned piece) -> void;

    // Floating-point numbers.
    auto write(Chain* chain, float piece) -> void;
    auto write(Chain* chain, double piece) -> void;

    // Memory addresses.
    auto write(Chain* chain, void* piece) -> void;
    auto write(Chain* chain, char* piece) -> void;    // Write as pointer address.

    // Native strings.
    auto write(Chain* chain, char const* piece) -> void;    // Write as native string.
}

