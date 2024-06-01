#pragma once

namespace dewox::inline object
{
    template <typename Object> concept can_drop = requires { &Object::drop; };

    template <typename Object, typename... Arguments>
    constexpr auto create(auto (*into)(Object* result, Arguments...) -> void, Arguments... arguments) -> Object;

    template <typename Object>
    constexpr auto drop(Object* opt_object) -> void;
}

namespace dewox::inline object
{
    template <typename Object, typename... Arguments>
    inline constexpr auto create(auto (*into)(Object* result, Arguments... arguments) -> void, Arguments... arguments) -> Object
    {
        Object result;
        into(&result, arguments...);
        return result;
    }

    template <typename Object>
    inline constexpr auto drop(Object* opt_object) -> void
    {
        if constexpr (can_drop<Object>) {
            if (auto object = opt_object) {
                object->drop();
            } else {
                // Silently ignore nullptr objects.
            }
        } else {
            // Do nothing because Object does not need to be dropped.
        }
    }
}

