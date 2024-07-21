#pragma once
#include "string.hpp"
#include "chain.hpp"
#include "type.hpp"

namespace dewox
{
    struct Cell
    {
        alignas(8u) Identity data[2];
    };

    struct Entity { Cell cells[1]; };
    struct Trait { Entity entity; };
    struct Partition { Entity entity; };

    struct Graph
    {
        // An entity is a sequence of cells. A cell is 16-byte data aligned to 8 bytes.
        // A trait describes the layout of an entity.
        // The trait description expression is a string of characters. Each character represents a cell type.
        // The feature description expression is a string of characters. Each character represents a feature of entities of the trait.
        //
        // -- Cell Type Characters --
        // * "e": A pointer to an entity. The target entity will link back. (Entity)
        // * "b": A small buffer, which is an array of bytes that usually represents a short name. (Buffer)
        // * "p": A phantom buffer, which is an array of bytes that will be loaded on demand. (Phantom buffer)
        // * "o": A managed pointer to a C++ object. (Object pointer)
        // * "x": A cell of arbitrary 16-byte data. (eXtra data)
        //
        // -- Feature Characters --
        // * "r": Runtime only. The entities of the trait will not be saved.
        auto add_trait(String description, String features) -> Trait*;

        auto add_entity(Trait* trait, Partition* partition) -> Entity*;
        auto partition_chain(Partition* partition) -> Chain*;
        auto root_partition() -> Partition*;
    };
}

