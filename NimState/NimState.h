#pragma once

#include "Components/Board.h"
#include "Components/Rules.h"
#include "GamePlayer/GameState.h"
#include "ZHash.h"

#include <optional>

// A tic-tac-toe game state.

class NimState : public GamePlayer::GameState
{
public:
    using PlayerId = GamePlayer::GameState::PlayerId;

    // Constructor
    explicit NimState(Board const & board, Rules::Variation variation = Rules::Variation::DEFAULT, PlayerId nextPlayer = PlayerId::FIRST);

    // Destructor
    virtual ~NimState() = default;

    // Returns a fingerprint for this state. Overrides GameState::fingerprint().
    virtual uint64_t fingerprint() const override { return zHash_.value(); }

    // Returns the player whose turn it is. Overrides GameState::whoseTurn().
    virtual PlayerId whoseTurn() const override { return nextPlayer_; }

    // Returns the board.
    Board const& board() const { return board_; }

    // Returns true if the game is over.
    bool isGameOver() const { return board_.empty(); }

    // Returns the winner of the game, if any.
    std::optional<PlayerId> winner() const;

    // Returns the last move made, if any.
    std::optional<std::pair<int, int>> lastMove() const { return lastMove_; }

    // Makes a move on the board by removing `n` objects from heap `i`.
    void move(int i, int n);

private:
    Board                               board_;         // Board stored in row-major order
    Rules::Variation                    variation_;     // Variation of the game rules
    PlayerId                            nextPlayer_;    // Next player to move
    ZHash                               zHash_;         // Zobrist hash for the game state
    std::optional<std::pair<int, int>>  lastMove_;      // Last move made (heap index and number of objects removed)
};
