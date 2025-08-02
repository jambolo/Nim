#include "gtest/gtest.h"

#include "ComputerPlayer/ComputerPlayer.h"
#include "Components/Board.h"
#include "Components/Rules.h"
#include "NimState/NimState.h"
#include <numeric>

// Helper function to check if two boards have exactly one heap difference
static bool exactlyOneDifference(Board const & board1, Board const & board2)
{
    const auto & heaps1 = board1.heaps();
    const auto & heaps2 = board2.heaps();

    if (heaps1.size() != heaps2.size()) {
        return false;
    }

    return std::transform_reduce(
        heaps1.begin(), heaps1.end(), heaps2.begin(),
        0,                                    // initial value
        std::plus<>(),                        // reduction operation
        [](int a, int b) { return a != b; }   // transform operation
    ) == 1;
}

namespace Nim
{

TEST(ComputerPlayer, Constructor)
{
    // Nothing to test here, just make sure the constructor executes without error.
    // There are only 4 combinations, so let's test them all.
    ASSERT_NO_THROW(ComputerPlayer(NimState::PlayerId::FIRST, Rules::Variation::MISERE));
    ASSERT_NO_THROW(ComputerPlayer(NimState::PlayerId::FIRST, Rules::Variation::NORMAL));
    ASSERT_NO_THROW(ComputerPlayer(NimState::PlayerId::SECOND, Rules::Variation::MISERE));
    ASSERT_NO_THROW(ComputerPlayer(NimState::PlayerId::SECOND, Rules::Variation::NORMAL));
}

TEST(ComputerPlayer, Move)
{
    ComputerPlayer computer1(NimState::PlayerId::FIRST);
    ComputerPlayer computer2(NimState::PlayerId::SECOND);
    Board board0({ 3, 4, 5 });
    NimState state(board0);

    // Move until the board is empty
    while (!state.isGameOver())
    {
        computer1.move(&state);
        Board board1 = state.board();
        ASSERT_TRUE(exactlyOneDifference(board0, board1));   // Check that exactly one heap has changed
        if (state.isGameOver())
            break;

        computer2.move(&state);
        Board board2 = state.board();
        ASSERT_TRUE(exactlyOneDifference(board1, board2));   // Check that exactly one heap has changed
        board0 = board2;
    }
}

} // namespace Nim
