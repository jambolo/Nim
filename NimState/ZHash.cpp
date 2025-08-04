#include "ZHash.h"

#include "Components/Board.h"
#include "GamePlayer/GameState.h"

#include <cassert>
#include <random>

ZHash::ZValueTable const ZHash::zValueTable_;

ZHash::ZHash(Board const & board, GamePlayer::GameState::PlayerId nextPlayer)
    : value_(ZHash::EMPTY)
{
    // Initialize the hash value based on the current board state
    for (int i = 0; i < board.size(); ++i)
    {
        // Assumes that the hash value for an empty heap is always 0
        int n = board.heap(i);
        value_ ^= zValueTable_.heap_[i][n];
    }

    // The hash for the first player is 0.
    if (nextPlayer == GamePlayer::GameState::PlayerId::SECOND)
        value_ ^= zValueTable_.nextPlayer_;
}

// Updates the hash value when changing the number of objects in heap 'i' from 'from' to 'to'.
ZHash ZHash::changeHeap(int i, int from, int to)
{
    assert(0 <= i && i < Board::MAX_HEAPS);
    assert(0 <= from && from <= Board::MAX_OBJECTS);
    assert(0 <= to && to <= Board::MAX_OBJECTS);
    value_ ^= zValueTable_.heap_[i][from];
    value_ ^= zValueTable_.heap_[i][to];
    return *this; // Return the updated ZHash object
}

ZHash ZHash::changeNextPlayer()
{
    value_ ^= zValueTable_.nextPlayer_;
    return *this; // Return the updated ZHash object
}

ZHash::ZValueTable::ZValueTable()
{
    std::mt19937_64 rng;
    static_assert(sizeof(std::mt19937_64::result_type) == 8, "The random number generator must generate 64 bits.");
    // Note: We don't seed the generator. This means that the same values are generated each time the program is run.

    for (int i = 0; i < Board::MAX_HEAPS; ++i)
    {
        heap_[i][0] = ZHash::EMPTY; // The hash value for an empty heap is always 0
        for (int j = 1; j <= Board::MAX_OBJECTS; ++j)
        {
            heap_[i][j] = rng(); // Generate a random hash value for each heap and number of objects
        }
    }
    nextPlayer_ = rng(); // Generate a random hash value for the next player
}
