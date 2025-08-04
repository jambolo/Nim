#include "gtest/gtest.h"

#include "Components/Board.h"

namespace Nim
{

TEST(Board, Constructor)
{
    ASSERT_NO_THROW(Board({1, 2, 3}));
    ASSERT_NO_THROW(Board({0, 0, 0}));                                // Empty heaps are allowed
    ASSERT_NO_THROW(Board({Board::MAX_OBJECTS}));                     // Maximum number of objects in heaps
    ASSERT_NO_THROW(Board(std::vector<int8_t>(Board::MAX_HEAPS, 1))); // Maximum number of heaps
    EXPECT_DEATH(Board({}), "Assertion failed: .*");                  // Board with no heaps
    EXPECT_DEATH(Board(std::vector<int8_t>(Board::MAX_HEAPS + 1, 1)), "Assertion failed: .*"); // Too many heaps
    EXPECT_DEATH(Board({-1, 2, 3}), "Assertion failed: .*");               // Negative number of objects in a heap
    EXPECT_DEATH(Board({Board::MAX_OBJECTS + 1}), "Assertion failed: .*"); // Too many objects in a heap
}

TEST(Board, Heaps)
{
    std::vector<int8_t> heaps = {1, 2, 3};
    EXPECT_EQ(Board(heaps).heaps(), heaps); // Check if heaps are returned correctly
}

TEST(Board, Size)
{
    std::vector<int8_t> heaps = {1, 2, 3};
    EXPECT_EQ(Board(heaps).size(), heaps.size()); // Check if size is returned correctly
}

TEST(Board, Empty)
{
    EXPECT_TRUE(Board({0, 0, 0}).empty()); // Check if an empty board is recognized as empty
}

TEST(Board, Heap)
{
    std::vector<int8_t> heaps = {1, 2, 3};
    Board               board(heaps);
    for (size_t i = 0; i < heaps.size(); ++i)
    {
        EXPECT_EQ(board.heap(i), heaps[i]); // Check if the number of objects in each heap is returned correctly
    }
    EXPECT_DEATH(board.heap(-1), "Assertion failed: .*");               // Attempt to access an invalid should fail
    EXPECT_DEATH(board.heap(heaps.size() + 1), "Assertion failed: .*"); // Attempt to access an invalid should fail
}

TEST(Board, Remove)
{
    std::vector<int8_t> heaps = {1, 2, 3};
    Board               board(heaps);
    EXPECT_TRUE(board.remove(0, 1) == 0 && board.heap(0) == 0); // Remove 1 from heap 0 leaving it empty
    EXPECT_TRUE(board.remove(1, 1) == 1 && board.heap(1) == 1); // Remove 1 from heap 1 leaving 1
    EXPECT_DEATH(board.remove(0, 1), "Assertion failed: .*");   // Attempt to remove from an empty heap should fail
    EXPECT_DEATH(board.remove(1, 2), "Assertion failed: .*");   // Attempt to remove more than available should fail
    EXPECT_DEATH(board.remove(2, -1), "Assertion failed: .*");  // Attempt to remove a negative number should fail
    EXPECT_DEATH(board.remove(2, 0), "Assertion failed: .*");   // Attempt to remove 0 should fail
    EXPECT_DEATH(board.remove(-1, 1), "Assertion failed: .*");  // Attempt to remove from an invalid heap should fail
    EXPECT_DEATH(board.remove(99, 1), "Assertion failed: .*");  // Attempt to remove from an invalid heap should fail
}

} // namespace Nim
