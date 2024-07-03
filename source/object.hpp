#pragma once
// Conventions to define your own object struct, and helper functions to utilize conventions.
#include "tool.hpp"

namespace dewox::inline object
{
    template <typename Object> concept sinking_type = requires (Object* a) { a->drop(); };

    template <mutable_type Object, typename... Arguments>
    constexpr auto create(auto (*into)(Object* result, Arguments...) -> void, Self<Arguments>... arguments) -> Object;

    template <mutable_type Object>
    constexpr auto drop(Object* object) -> void;

    template <sinking_type Object>
    constexpr auto reset(Object* object) -> void;

    template <sinking_type Object>
    struct Sink
    {
        Object object{};

        ~Sink();
        Sink();
        Sink(Sink&& a);
        auto operator = (Sink a) -> Sink&;

        static auto into(Sink* result, Object object) -> void;

        auto operator * () -> Object*;
        auto operator -> () -> Object*;
    };
}

namespace dewox::inline object
{
    template <mutable_type Object, typename... Arguments>
    inline constexpr auto create(auto (*into)(Object* result, Arguments... arguments) -> void, Self<Arguments>... arguments) -> Object
    {
        Object result;
        into(&result, arguments...);
        return result;
    }

    template <mutable_type Object>
    inline constexpr auto drop(Object* object) -> void
    {
        if constexpr (sinking_type<Object>) {
            object->drop();
        }
    }

    template <sinking_type Object>
    inline constexpr auto reset(Object* object) -> void
    {
        object->drop();
        *object = {};
    }

    template <sinking_type Object> inline Sink<Object>::~Sink() { object.drop(); }
    template <sinking_type Object> inline Sink<Object>::Sink() = default;
    template <sinking_type Object> inline Sink<Object>::Sink(Sink&& a): object{exchange(&a.object, {})} {}
    template <sinking_type Object> inline auto Sink<Object>::operator = (Sink a) -> Sink& { swap(&object, &a.object); return *this; }

    template <sinking_type Object> inline auto Sink<Object>::into(Sink* result, Object object) -> void { result->object = object; }

    template <sinking_type Object> inline auto Sink<Object>::operator *  () -> Object* { return &object; }
    template <sinking_type Object> inline auto Sink<Object>::operator -> () -> Object* { return &object; }
}

