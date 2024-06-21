#pragma once
#include "string.hpp"
#include "slice.hpp"
#include "type.hpp"

namespace dewox::inline terminal
{
    struct Option_Description
    {
        String name;
        String maybe_short_name;
        String description;
        Size num_arguments;
    };

    using Find_Option = auto (String command, String opt_argument, void* opt_data) -> bool;
    using Find_Line = auto (String line, void* opt_data) -> bool;

    auto parse_option(Slice<Option_Description> options) -> void;

    auto count_terminal_cells(String maybe_text) -> Size;
    auto break_into_terminal_lines(String maybe_text, Size num_cells_per_line, Find_Line* find, void* opt_data) -> bool;
}

