#include "NimEvaluator.h"

#include "NimState/NimState.h"

#include <algorithm>
#include <cassert>
#include <numeric>

static NimState::PlayerId otherPlayer(NimState::PlayerId player)
{
    return (player == NimState::PlayerId::FIRST) ? NimState::PlayerId::SECOND : NimState::PlayerId::FIRST;
}

NimEvaluator::NimEvaluator(Rules rules)
    : GamePlayer::StaticEvaluator()
    , rules_(std::move(rules))
{
}

float NimEvaluator::evaluate(GamePlayer::GameState const & state) const
{
    auto const & nimState = dynamic_cast<NimState const &>(state);

    float score = 0.0f;

    // If the game is over, then return the score for the winner
    if (nimState.isGameOver())
    {
        score = (nimState.winner().value() == GamePlayer::GameState::PlayerId::FIRST) ? WIN_VALUE : -WIN_VALUE;
    }
    // Otherwise, evaluate the state based on the variation.
    else
    {
        switch (rules_.variation())
        {
        case Rules::Variation::MISERE:
            score = evaluateMisere(nimState);
            break;
        case Rules::Variation::NORMAL:
            score = evaluateNormal(nimState);
            break;
        case Rules::Variation::SUBTRACT:
            score = evaluateSubtract(nimState);
            break;
        default:
            assert(false && "Unknown variation");
            break;
        }
    }

    // Modify the score slightly based on the number of objects removed by the move.
    // This is an optimization. If the responses are sorted, then bigger moves are prioritized and this can reduce both the
    // branching and depth of the tree.
    auto lastMove = nimState.lastMove();
    if (lastMove.has_value())
    {
        score += (nimState.whoseTurn() == GamePlayer::GameState::PlayerId::FIRST) ? -float(lastMove->n) : float(lastMove->n);
    }
    return score;
}

float NimEvaluator::evaluateMisere(NimState const & state) const
{
    auto const & board             = state.board();
    auto         player            = otherPlayer(state.whoseTurn()); // Player who made the move
    bool         playerIsFirst     = (player == GamePlayer::GameState::PlayerId::FIRST);
    float        winningStateValue = playerIsFirst ? LIKELY_WIN_VALUE : -LIKELY_WIN_VALUE;
    float        losingStateValue  = -winningStateValue;

    // In the misère variation, evaluation is based on the nim-sum (as in the normal variation) until there 0 or 1 heaps with more
    // than 1 object.

    int significantHeaps = std::count_if(board.heaps().begin(), board.heaps().end(), [](int n) { return n > 1; });

    // If there is exactly one heap with a size greater than one, then the state is a losing state.
    if (significantHeaps == 1)
    {
        return losingStateValue;
    }

    // If there are no significant heaps, then all that are left are heaps of size 1. The winner depends on the number of remaining
    // heaps. The state is a losing state if there is an even number of heaps (nim-sum == 0), otherwise it is a winning state.
    if (significantHeaps == 0)
    {
        return (board.nimSum() == 0) ? losingStateValue : winningStateValue;
    }

    // In the general case, if the nim-sum is zero, then the state is a winning state, otherwise it is a losing state.
    return (board.nimSum() == 0) ? winningStateValue : losingStateValue;
}

float NimEvaluator::evaluateNormal(NimState const & state) const
{
    auto const & board             = state.board();
    auto         player            = otherPlayer(state.whoseTurn()); // Player who made the move
    bool         playerIsFirst     = (player == GamePlayer::GameState::PlayerId::FIRST);
    float        winningStateValue = playerIsFirst ? LIKELY_WIN_VALUE : -LIKELY_WIN_VALUE;
    float        losingStateValue  = -winningStateValue;

    return (board.nimSum() == 0) ? winningStateValue : losingStateValue;
}

float NimEvaluator::evaluateSubtract(NimState const & state) const
{
    auto const & board             = state.board();
    auto         player            = otherPlayer(state.whoseTurn()); // Player who made the move
    bool         playerIsFirst     = (player == GamePlayer::GameState::PlayerId::FIRST);
    float        winningStateValue = playerIsFirst ? LIKELY_WIN_VALUE : -LIKELY_WIN_VALUE;
    float        losingStateValue  = -winningStateValue;

    if (rules_.removalLimit() > 1)
    {
        return 0.0f;
    }
    return (board.heap(0) % (rules_.removalLimit() + 1) == 0) ? winningStateValue : losingStateValue;
}
