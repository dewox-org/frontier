#include "chain.hpp"
#include "object.hpp"
#include "string.hpp"
#include "type.hpp"
#include "tool.hpp"
#include "native.hpp"

namespace dewox::inline chain
{
    inline namespace
    {
        constexpr auto first_block_capacity = 256u;

        constexpr auto is_power_of_two(Size a) -> bool
        {
            return ((a & (a - 1u)) == 0u);
        }

        constexpr auto next_align(Size a, Size alignment) -> Size
        {
            return a + ((1u + ~a) & (alignment - 1u));
        }

        constexpr auto connect_link(auto* a, auto* b) -> void
        {
            a->next = b;
            b->back = a;
        }

        struct Block
        {
            /// [invariant] alignof(buffer()) == capacity;
            /// [invariant] byte_count + sizeof(Block) <= capacity;
            /// [invariant] next->capacity >= 2 * this->capacity; (when next != control_block)
            /// [invariant] byte_count > 0u;

            Block* back;
            Block* next;
            Size capacity;
            Size byte_count;

            static auto into(Block* result, Size capacity, Size byte_count) -> void
            {
                connect_link(result, result);
                result->capacity = capacity;
                result->byte_count = byte_count;
            }

            static auto after_into(Block* result, Block* reference, Size capacity, Size byte_count) -> void
            {
                connect_link(result, reference->next);
                connect_link(reference, result);
                result->capacity = capacity;
                result->byte_count = byte_count;
            }

            auto drop() -> void
            {
                connect_link(back, next);
                native::drop_memory(buffer().begin());
            }

            auto buffer() -> String
            {
                return create(
                    &String::count_into
                    , (char*) this + sizeof(Block) - capacity
                    , byte_count
                );
            }
        };

        auto create_block_after(Block* reference, Size capacity, Size byte_count) -> Block*
        {
            if (!is_power_of_two(capacity)) native::fatal();
            auto data = native::grow_memory(capacity, capacity);
            auto block = (Block*) (data + capacity - sizeof(Block));
            Block::after_into(block, reference, capacity, byte_count);
            return block;
        }

        struct Control
        {
            /// [invariant] next_grow_alignment <= block.back->capacity / 2;

            Size next_grow_alignment;
            Size total_byte_count;
            Size reserved_1;
            Size reserved_2;
            Block block;

            static auto into(Control* result) -> void
            {
                result->next_grow_alignment = 1u;
                result->total_byte_count = 0u;
                Block::into(&result->block, sizeof(Control), sizeof(Control) - sizeof(Block));
            }

            auto drop() -> void
            {
                while (block.back != &block) {
                    block.back->drop();
                }
                block.drop();
            }

            auto grow(Size byte_count, Size alignment) -> String
            {
                if (byte_count) {
                    total_byte_count += byte_count;
                    alignment = max(exchange(&next_grow_alignment, 1u), alignment);
                    if (fit(byte_count, alignment)) {
                        auto back = block.back;
                        auto aligned_byte_count = next_align(back->byte_count, alignment);
                        total_byte_count += aligned_byte_count - back->byte_count;
                        back->byte_count = aligned_byte_count + byte_count;
                        return back->buffer().skip(aligned_byte_count);
                    } else {
                        auto back = create_block_after(block.back, max(max(alignment << 1u, first_block_capacity), block.back->capacity << 1u), byte_count);
                        return back->buffer();
                    }
                } else {
                    next_grow_alignment = max(next_grow_alignment, alignment);
                    return {};
                }
            }

            auto pop(Size byte_count) -> void
            {
                while (block.back != &block) {
                    auto back = block.back;
                    if (byte_count < back->byte_count) {
                        back->byte_count -= byte_count;
                        total_byte_count -= byte_count;
                        break;
                    } else {
                        byte_count -= back->byte_count;
                        total_byte_count -= back->byte_count;
                        back->drop();
                    }
                }
            }

            auto merge(Size byte_count) -> String
            {
                if (total_byte_count == 0u) return {};
                byte_count = min(byte_count, total_byte_count);

                auto back = block.back;
                if (back->byte_count >= byte_count) {
                    return back->buffer().suffix(byte_count);
                } else {
                    auto merged_block = create_block_after(back, back->capacity << 1u, byte_count);
                    auto merged_buffer = merged_block->buffer();
                    while (merged_buffer) {
                        auto merged_buffer_byte_count = merged_buffer.byte_count();
                        if (back->byte_count > merged_buffer_byte_count) {
                            merged_buffer.copy(back->buffer().suffix(merged_buffer_byte_count));
                            merged_buffer = {};
                            back->byte_count -= merged_buffer_byte_count;
                        } else {
                            merged_buffer = merged_buffer.pop(merged_buffer.copy(back->buffer()).byte_count());
                            back->drop();
                            back = merged_block->back;
                        }
                    }
                    return merged_block->buffer();
                }
            }

            auto fit(Size byte_count, Size alignment) -> bool
            {
                auto b = block.back;
                if (b == &block) return false;
                if (alignment > (b->capacity >> 1u)) return false;
                if (next_align(b->byte_count, alignment) + byte_count + sizeof(Block) > b->capacity) return false;
                return true;
            }
        };

        auto create_control() -> Control*
        {
            // NOTE: The control must be aligned to the sizeof(Control), not the alignof(Control).
            auto control = (Control*) native::grow_memory(sizeof(Control), sizeof(Control));
            Control::into(control);
            return control;
        }

        static_assert(first_block_capacity >= 2u * sizeof(Block));
        static_assert(alignof(Block) == alignof(Size));
        static_assert(alignof(Block) == alignof(Block*));
        static_assert(is_power_of_two(sizeof(Control)));
    }

    auto Chain::drop() -> void
    {
        if (auto control = (Control*) maybe_control) {
            control->drop();
        } else {
            // Nothing to be done.
        }
    }

    auto Chain::grow(Size byte_count, Size alignment) -> String
    {
        if (!is_power_of_two(alignment)) native::fatal();

        if (!maybe_control) maybe_control = create_control();
        auto control = (Control*) maybe_control;

        return control->grow(byte_count, alignment);
    }

    auto Chain::pop(Size byte_count) -> void
    {
        if (auto control = (Control*) maybe_control) {
            control->pop(byte_count);
        } else {
            // Nothing to be done.
        }
    }

    auto Chain::merge(Size byte_count) -> String
    {
        if (auto control = (Control*) maybe_control) {
            return control->merge(byte_count);
        } else {
            return {};
        }
    }

    auto Chain::front() -> char*
    {
        if (auto control = (Control*) maybe_control) {
            if (auto block = control->block.next; block != &control->block) {
                return block->buffer().begin();
            } else {
                return {};
            }
        } else {
            return {};
        }
    }

    auto Chain::byte_count() -> Size
    {
        if (auto control = (Control*) maybe_control) {
            return control->total_byte_count;
        } else {
            return {};
        }
    }

    auto Chain::search(Find_Buffer* find, void* maybe_data) -> bool
    {
        if (auto control = (Control*) maybe_control) {
            for (auto block = control->block.next, control_block = &control->block; block != control_block; block = block->next) {
                if (find(block->buffer(), maybe_data)) {
                    return true;
                }
            }
            return false;
        } else {
            return false;
        }
    }
}

