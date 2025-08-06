#include "gtest/gtest.h"

#include "Components/Rules.h"
#include "HumanPlayer/HumanPlayer.h"
#include "NimState/NimState.h"

namespace Nim
{
TEST(HumanPlayer, Constructor)
{
    // Nothing to test here, just make sure the constructor executes without error
    Rules rules;
    ASSERT_NO_THROW(HumanPlayer(NimState::PlayerId::FIRST, rules));
    ASSERT_NO_THROW(HumanPlayer(NimState::PlayerId::SECOND, rules));
}

TEST(HumanPlayer, Move)
{
    // I'm not motivated to implement this test right now.
}
} // namespace Nim
