# SYNOPSIS

Object model.

1. No destructors. No constructors. Always movable. Always copyable.
2. Zero-initialize to valid state.
3. No C++ references.

    // 4. Drop internal resources.
    struct Foo { auto drop() -> void; };

    // 5. auto drop().
    auto foo = Sink<Foo>{};

