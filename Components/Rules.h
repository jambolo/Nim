#pragma once

// Various rules for the game

#include <cstdint>
#include <limits>

class Rules
{
public:
    // Different variations of the game
    enum class Variation
    {
        MISERE = 0,      // Misère
        NORMAL,          // Normal
        SUBTRACT,     // Subtraction
        DEFAULT = MISERE // Default variation
    };

    explicit Rules(Variation variation = Variation::DEFAULT, int removalLimit = std::numeric_limits<int8_t>::max())
        : variation_(variation)
        , removalLimit_(removalLimit)
    {
    }
    Variation variation() const { return variation_; }
    int       removalLimit() const { return removalLimit_; }

private:
    Variation variation_;    // Variation of the game
    int       removalLimit_; // Maximum number of objects that can be removed from a heap
};
