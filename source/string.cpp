#include "string.hpp"
#include "native.hpp"
#include "chain.hpp"

namespace dewox
{
    inline namespace
    {
        auto valid_pattern(String pattern) -> bool
        {
            auto skip_count = Size{};
            for (auto command: pattern) {
                if (skip_count > 0u) {
                    skip_count--;
                    continue;
                }
                switch (command) {
                    case '~': skip_count = 2u; break;
                    case '@': skip_count = 1u; break;
                    case ':': skip_count = 1u; break;
                    default: break;
                }
            }
            return (skip_count == 0u);
        }

        // Return true if matched.
        auto do_match(String* source, String pattern, char const* maybe_subpattern = {}) -> bool
        {
            trace("match-subpattern");
            auto subpattern = (maybe_subpattern ? maybe_subpattern : pattern.begin());
            auto command = subpattern;
            auto peek = source->begin();
            auto matched = true;

            auto fail_branch = [&] {
                while (command < pattern.end()) {
                    switch (*command) {
                        case '~': command += 3u; break;
                        case '@': command += 2u; break;
                        case ':': matched = false; return;
                        case '|': command += 1u; return;
                        default: command += 1u; break;
                    }
                }
                matched = false;
            };

            auto try_find_subpattern = [&pattern] (char name, char const* self_subpattern) -> char const* {
                if (name == '@') return self_subpattern;
                for (auto command = pattern.begin(); command < pattern.end(); ) {
                    switch (*command) {
                        case '~': command += 3u; break;
                        case '@': command += 2u; break;
                        case ':': {
                            command += 2u;
                            if (command[-1] == name) return command;
                            break;
                        }
                        case '|': command += 1u; break;
                        default: command += 1u; break;
                    }
                }
                return {};
            };

            while (command < pattern.end()) {
                switch (*command) {
                    case '~': {
                        trace("~");
                        command += 3u;
                        if (peek < source->end()) {
                            if (command[-2] <= *peek && *peek <= command[-1]) {
                                peek++;
                            } else {
                                fail_branch();
                            }
                        } else {
                            fail_branch();
                        }
                        break;
                    }

                    case '@': {
                        trace("@");
                        command += 2u;
                        if (auto next_subpattern = try_find_subpattern(command[-1], subpattern)) {
                            auto s = create(String::into, peek, source->end());
                            if (do_match(&s, pattern, next_subpattern)) {
                                peek = s.begin();
                            } else {
                                fail_branch();
                            }
                        } else {
                            fail_branch();
                        }
                        break;
                    }

                    case ':': trace("end-:"); command = pattern.end(); break;    // End of current branch
                    case '|': trace("end-|"); command = pattern.end(); break;    // End of current branch

                    default: {
                        trace("match-char");
                        command += 1u;
                        if (peek < source->end()) {
                            if (*peek == command[-1]) {
                                peek++;
                            } else {
                                fail_branch();
                            }
                        } else {
                            fail_branch();
                        }
                        break;
                    }
                }
            }
            if (matched) source->first = peek;
            trace((matched ? "match = yes" : "match = no"));
            char at[] = "match at '.'";
            at[10] = *peek;
            trace(at);
            return matched;
        }
    }

    auto String::match(String pattern) -> Size
    {
        if (!valid_pattern(pattern)) return {};

        auto pending = *this;
        if (do_match(&pending, pattern)) {
            return Size(pending.begin() - begin());
        } else {
            return {};
        }
    }

    auto String::search(String pattern) -> String
    {
        if (!valid_pattern(pattern)) return {};

        auto pending = *this;
        while (pending) {
            auto pending_to_match = pending;
            if (do_match(&pending_to_match, pattern)) {
                pending = pending.pop(pending_to_match.byte_count());
                break;
            } else {
                pending = pending.skip(1u);
            }
        }
        return pending;
    }

    auto String::check_bounds() -> void
    {
        if (first > last) {
            (void) native::fatal();
            return;
        }
    }
}

namespace dewox
{
    auto write(Chain* chain, String piece) -> void
    {
        chain->grow(piece.byte_count()).copy(piece);
    }
}

