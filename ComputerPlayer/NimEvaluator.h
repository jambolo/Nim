#pragma once

#include "GamePlayer/StaticEvaluator.h"

namespace GamePlayer
{
class GameState;
}

// A static evaluation function for tic-tac-toe.
class NimEvaluator : public GamePlayer::StaticEvaluator
{
public:
    // Constructor.
    NimEvaluator() = default;

    // Destructor.
    virtual ~NimEvaluator() = default;

    // Returns a value for the given state. Overrides StaticEvaluator::evaluate().
    virtual float evaluate(GamePlayer::GameState const & state) const override;

    // Returns the value of a winning state for the first player. Overrides StaticEvaluator::firstPlayerWins().
    virtual float firstPlayerWins() const override { return FIRST_PLAYER_WIN_VALUE; }

    // Returns the value of a winning state for the second player. Overrides StaticEvaluator::secondPlayerWins().
    virtual float secondPlayerWins() const override { return SECOND_PLAYER_WIN_VALUE; }

private:
    // Value constants for evaluation
    static float constexpr FIRST_PLAYER_WIN_VALUE   = 10000.0f;
    static float constexpr SECOND_PLAYER_WIN_VALUE  = -10000.0f;
};
