#include "gtest/gtest.h"

#include "ComputerPlayer/NimEvaluator.h"
#include "NimState/NimState.h"
#include "Components/Board.h"

namespace Nim
{

TEST(NimEvaluator, Constructor)
{
    // Nothing to test here, just make sure the constructor executes without error
    ASSERT_NO_THROW(NimEvaluator());
}

TEST(NimEvaluator, Evaluate)
{
    // I don't know how to test this yet, so just make sure it runs without error.
    NimEvaluator evaluator;
    NimState state(Board({ 1, 2, 3 }));
    ASSERT_NO_THROW(evaluator.evaluate(state));
}

} // namespace Nim
