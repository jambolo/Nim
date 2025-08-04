#include "gtest/gtest.h"

#include "Components/Board.h"
#include "Components/Rules.h"
#include "ComputerPlayer/NimEvaluator.h"
#include "NimState/NimState.h"

namespace Nim
{

TEST(NimEvaluator, Constructor)
{
    // Nothing to test here, just make sure the constructor executes without error
    ASSERT_NO_THROW(NimEvaluator{Rules::Variation::MISERE});
    ASSERT_NO_THROW(NimEvaluator{Rules::Variation::NORMAL});
}

TEST(NimEvaluator, Evaluate)
{
    // I don't know how to test this yet, so just make sure it runs without error.
    NimEvaluator evaluator(Rules::Variation::DEFAULT);
    NimState     state(Board({1, 2, 3}));
    ASSERT_NO_THROW(evaluator.evaluate(state));
}

} // namespace Nim
