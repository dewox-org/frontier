#pragma once
#include "tool.hpp"

namespace dewox::inline object
{
    template <typename Object> concept can_drop = requires { &Object::drop; };

    template <typename Object, typename... Arguments>
    constexpr auto create(auto (*into)(Object* result, Arguments...) -> void, Self<Arguments>... arguments) -> Object;

    template <typename Object>
    constexpr auto drop(Object* maybe_object) -> void;
}

namespace dewox::inline object
{
    template <typename Object, typename... Arguments>
    inline constexpr auto create(auto (*into)(Object* result, Arguments... arguments) -> void, Self<Arguments>... arguments) -> Object
    {
        Object result;
        into(&result, arguments...);
        return result;
    }

    template <typename Object>
    inline constexpr auto drop(Object* maybe_object) -> void
    {
        if constexpr (can_drop<Object>) {
            if (auto object = maybe_object) {
                object->drop();
            } else {
                // Silently ignore nullptr objects.
            }
        } else {
            // Do nothing because Object does not need to be dropped.
        }
    }
}

