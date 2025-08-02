#include "NimState.h"

#include "ZHash.h"

#include "Components/Board.h"

#include <cassert>
#include <optional>

NimState::NimState(Board const &    board,
                   Rules::Variation variation/* = Rules::Variation::DEFAULT*/,
                   PlayerId         nextPlayer/* = PlayerId::FIRST*/)
    : board_(board)
    , variation_(variation)
    , nextPlayer_(nextPlayer)
    , zHash_(board, nextPlayer)
    , lastMove_(std::nullopt)
{

}

std::optional<NimState::PlayerId> NimState::winner() const
{
    // If the game is not over, return no winner
    if (!isGameOver())
        return std::nullopt;

    switch (variation_)
    {
    case Rules::Variation::MISERE: return nextPlayer_;                                                              // The player who did not make the last move wins
    case Rules::Variation::NORMAL: return (nextPlayer_ == PlayerId::FIRST) ? PlayerId::SECOND : PlayerId::FIRST;    // The player who made the last move wins
    default:
        assert(false && "Unknown variation of the game rules");
        return std::nullopt; // Return no winner if the variation is unknown
    }
}

// Makes a move on the board by removeing `n` objects from heap `i`.
void NimState::move(int i, int n)
{
    assert(0 <= i && i < board_.size() && 0 < n && n <= board_.heap(i)); // Ensure the move is valid

    int from = board_.heap(i); // Get the current number of objects in heap `i`
    int to   = from - n;       // Get the new number of objects in heap `i`
    board_.remove(i, n); // Update the heap
    zHash_.changeHeap(i, from, to); // Update the Zobrist hash for the heap change

    nextPlayer_ = (nextPlayer_ == PlayerId::FIRST) ? PlayerId::SECOND : PlayerId::FIRST; // Switch players
    zHash_.changeNextPlayer(); // Update the Zobrist hash for the player change

    lastMove_ = std::make_pair(i, n); // Store the last move made
}
