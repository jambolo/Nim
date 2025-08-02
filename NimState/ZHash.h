#pragma once

#include "Components/Board.h"
#include "GamePlayer/GameState.h"

#include <cstdint>

// Zobrist Hashing Calculator.
//
// Zobrist Hashing is used to encode the state of a chess game in a single number. There is a risk of two unrelated states
// having the same value. In this implementation, the value has 64 bits and the risk of collision can be assumed to be
// very low, though not for a large number of values. The probability of at least one collision with 1 million values
// is 2.710502×10<sup>-8</sup>, but with 1 billion values, it rises to 1 in 40.
//
// An important characteristic of a Zorbrist hash is that it is independent of the order of the changes made to reach the state.
class ZHash
{
public:

    // Type of a hash value
    using Z = std::uint64_t;

    // The value of an empty board
    static Z constexpr EMPTY = 0;

    // A value which represents an "undefined" state
    static Z constexpr UNDEFINED = ~EMPTY;

    // Constructor
    explicit ZHash(Z z = EMPTY)
        : value_(z)
    {
    }

    // Constructor
    ZHash(Board const &                   board,
          GamePlayer::GameState::PlayerId currentPlayer);

    // Returns the current value.
    Z value() const { return value_; }

    // Returns true if the value is undefined (i.e. not a legal Z value)
    bool isUndefined() const { return value_ == UNDEFINED; }

    // Updates the hash value when changing the number of objects in heap 'i' from 'from' to 'to'. Returns the updated ZHash object.
    ZHash changeHeap(int i, int from, int to);

    // Updates the hash value when changing the next player. Returns the updated ZHash object.
    ZHash changeNextPlayer();

private:

    friend bool operator ==(ZHash const & x, ZHash const & y);
    friend bool operator <(ZHash const & x, ZHash const & y);

    class ZValueTable;                     // declared below

    Z value_;                              // The hash value

    static ZValueTable const zValueTable_; // The hash values for each incremental state change
};

// Equality operator
inline bool operator ==(ZHash const & x, ZHash const & y)
{
    return x.value_ == y.value_;
}

// Less than operator
inline bool operator <(ZHash const & x, ZHash const & y)
{
    return x.value_ < y.value_;
}

class ZHash::ZValueTable
{
public:

    ZValueTable();

    Z heap_[Board::MAX_HEAPS][Board::MAX_OBJECTS + 1];  // The hash value for heap i with j objects
    Z nextPlayer_;                                      // The hash value for the next player
};
