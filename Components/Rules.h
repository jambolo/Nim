#pragma once

// Various rules for the game

#include <cstdint>

namespace Rules
{

enum class Variation : int8_t
{
    MISERE = 0,      // Misère
    NORMAL,          // Normal
    DEFAULT = MISERE // Default variation
};

} // namespace Rules
