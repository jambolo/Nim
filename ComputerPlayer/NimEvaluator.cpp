#include "NimEvaluator.h"

#include "NimState/NimState.h"


float NimEvaluator::evaluate(GamePlayer::GameState const & state) const
{
    auto const &  nimState = dynamic_cast<NimState const &>(state);

    float score = 0.0f;

    return score;
}
