#include "gtest/gtest.h"

#include "NimState/NimState.h"

namespace Nim
{

TEST(NimState, Constructor)
{
    // Nothing to test here, just make sure the constructor executes without error
    NimState state (Board({1, 2, 3}));
}

TEST(NimState, Fingerprint)
{
    // Fingerprint values are determined at run-time, but the fingerprint of an empty board should be 0 and a non-empty board
    // should not be 0.
    {
        NimState emptyState(Board({ 0, 0, 0 }));
        EXPECT_EQ(emptyState.fingerprint(), 0);
    }
    {
        NimState nonEmptyState(Board({ 1, 2, 3 }));
        EXPECT_NE(nonEmptyState.fingerprint(), 0);
    }
}

TEST(NimState, WhoseTurn)
{
    EXPECT_EQ(NimState(Board({ 0, 0, 0 }), Rules::Variation::DEFAULT, NimState::PlayerId::FIRST).whoseTurn(), NimState::PlayerId::FIRST);
    EXPECT_EQ(NimState(Board({ 0, 0, 0 }), Rules::Variation::DEFAULT, NimState::PlayerId::SECOND).whoseTurn(), NimState::PlayerId::SECOND);
}

TEST(NimState, Board)
{
    {
        Board empty({ 0 });
        EXPECT_EQ(NimState(empty).board(), empty);
    }
    {
        Board board({ 1, 2, 3 });
        EXPECT_EQ(NimState(board).board(), board);
    }
}

TEST(NimState, IsGameOver)
{
    EXPECT_TRUE(NimState(Board({ 0 })).isGameOver());           // An empty board is game over
    EXPECT_FALSE(NimState(Board({ 1, 2, 3 })).isGameOver());    // A non-empty board is not game over
}

TEST(NimState, Winner)
{
    Board empty({ 0, 0, 0 });
    ASSERT_TRUE(NimState(empty).isGameOver()); // Sanity check: the game should be over when all heaps are empty

    // The winner depends on the variation of the game rules, but an empty board has no winner.
    EXPECT_FALSE(NimState(Board({ 1, 2, 3 })).winner());
    EXPECT_EQ(NimState(empty, Rules::Variation::MISERE, NimState::PlayerId::FIRST).winner(), NimState::PlayerId::FIRST);
    EXPECT_EQ(NimState(empty, Rules::Variation::MISERE, NimState::PlayerId::SECOND).winner(), NimState::PlayerId::SECOND);
    EXPECT_EQ(NimState(empty, Rules::Variation::NORMAL, NimState::PlayerId::FIRST).winner(), NimState::PlayerId::SECOND);
    EXPECT_EQ(NimState(empty, Rules::Variation::NORMAL, NimState::PlayerId::SECOND).winner(), NimState::PlayerId::FIRST);
}

TEST(NimState, LastMove)
{
    NimState state(Board({ 0, 0, 1 }));
    EXPECT_EQ(state.lastMove(), std::nullopt); // No moves made yet

    state.move(2, 1); // Make a move
    EXPECT_EQ(state.lastMove(), std::make_pair(2, 1)); // Last move should be (heap index 2, removed 1 object)
}

TEST(NimState, Move)
{
    NimState state(Board({ 1, 0, 1 }), Rules::Variation::DEFAULT, NimState::PlayerId::FIRST);
    uint64_t f0 = state.fingerprint(); // Store the initial fingerprint

    // If the first player removes 1 object from heap 2, the board should be {1, 0, 0}, the next player should be SECOND,
    // and the fingerprint should be updated.
    state.move(2, 1);
    EXPECT_EQ(state.board(), Board({ 1, 0, 0 }));               // The board should be {1, 0, 0}
    EXPECT_EQ(state.whoseTurn(), NimState::PlayerId::SECOND);   // The next player should be SECOND
    EXPECT_NE(state.fingerprint(), f0);                         // The fingerprint should have changed

    // If the second player removes the last object, the board should be empty, the game should be over, the next player
    // should be FIRST, and the fingerprint should be 0.
    state.move(0, 1);
    EXPECT_TRUE(state.board().empty());                         // The board should be empty
    EXPECT_TRUE(state.isGameOver());                            // The game should be over
    EXPECT_EQ(state.whoseTurn(), NimState::PlayerId::FIRST);    // The next player should be FIRST
    EXPECT_EQ(state.fingerprint(), 0);                          // The fingerprint should be 0
}

} // namespace Nim
