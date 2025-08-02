#include "gtest/gtest.h"

#include "Components/Rules.h"
#include "HumanPlayer/HumanPlayer.h"
#include "NimState/NimState.h"

namespace Nim
{
TEST(HumanPlayer, Constructor)
{
    // Nothing to test here, just make sure the constructor executes without error
    ASSERT_NO_THROW(HumanPlayer(NimState::PlayerId::FIRST, Rules::Variation::MISERE));
    ASSERT_NO_THROW(HumanPlayer(NimState::PlayerId::SECOND, Rules::Variation::NORMAL));
}

TEST(HumanPlayer, Move)
{
    // I'm not motivated to implement this test right now.
}
} // namespace Nim
