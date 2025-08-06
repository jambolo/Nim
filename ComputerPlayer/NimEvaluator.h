#pragma once

#include "Components/Rules.h"
#include "GamePlayer/StaticEvaluator.h"

namespace GamePlayer
{
class GameState;
}

class NimState;

// A static evaluation function for tic-tac-toe.
class NimEvaluator : public GamePlayer::StaticEvaluator
{
public:
    // Constructor.
    explicit NimEvaluator(Rules rules);

    // Destructor.
    virtual ~NimEvaluator() = default;

    // Returns a value for the given state. Overrides StaticEvaluator::evaluate().
    virtual float evaluate(GamePlayer::GameState const & state) const override;

    // Returns the value of a winning state for the first player. Overrides StaticEvaluator::firstPlayerWinsValue().
    virtual float firstPlayerWinsValue() const override { return WIN_VALUE; }

    // Returns the value of a winning state for the second player. Overrides StaticEvaluator::secondPlayerWinsValue().
    virtual float secondPlayerWinsValue() const override { return -WIN_VALUE; }

private:
    // Value constants for evaluation
    static float constexpr WIN_VALUE        = 10000.0f; // Value of a state that is a winning position
    static float constexpr LIKELY_WIN_VALUE = 5000.0f;  // Value of a state that is likely to be a winning position

    float evaluateMisere(NimState const & state) const;
    float evaluateNormal(NimState const & state) const;
    float evaluateSubtract(NimState const & state) const;

    Rules rules_; // The rules for the game being played
};
