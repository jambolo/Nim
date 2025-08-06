#include "gtest/gtest.h"

#include "Components/Board.h"
#include "Components/Rules.h"
#include "ComputerPlayer/NimEvaluator.h"
#include "NimState/NimState.h"

namespace Nim
{

TEST(NimEvaluator, Constructor)
{
    Rules rules;
    // Nothing to test here, just make sure the constructor executes without error
    ASSERT_NO_THROW(NimEvaluator{rules});
}

TEST(NimEvaluator, Evaluate)
{
    // I don't know how to test this yet, so just make sure it runs without error.
    Rules        rules;
    NimEvaluator evaluator(rules);
    NimState     state(Board({1, 2, 3}), rules);
    ASSERT_NO_THROW(evaluator.evaluate(state));
}

} // namespace Nim
