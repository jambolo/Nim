#include "gtest/gtest.h"

#include "Components/Board.h"
#include "NimState/NimState.h"
#include "NimState/ZHash.h"
#include <algorithm>
#include <vector>

static bool containsDuplicates(std::vector<ZHash> const & hashes)
{
    for (auto i = hashes.begin(); i != hashes.end(); ++i)
    {
        for (auto j = std::next(i); j != hashes.end(); ++j)
        {
            if (i->value() == j->value())
            {
                return true; // Found a duplicate
            }
        }
    }
    return false; // No duplicates found
}

namespace Nim
{

TEST(ZHash, Constructor_z)
{
    ASSERT_NO_THROW(ZHash());
    ASSERT_NO_THROW(ZHash{ZHash::EMPTY});
    ASSERT_NO_THROW(ZHash{ZHash::UNDEFINED});
    ASSERT_NO_THROW(ZHash(1));
    EXPECT_EQ(ZHash().value(), ZHash::EMPTY);
    EXPECT_EQ(ZHash(ZHash::EMPTY).value(), ZHash::EMPTY);
    EXPECT_EQ(ZHash(ZHash::UNDEFINED).value(), ZHash::UNDEFINED);
    EXPECT_EQ(ZHash(1).value(), 1);
}

TEST(ZHash, Constructor_board_player)
{
    // Make sure that the constructor works.
    ASSERT_NO_THROW(ZHash(Board({1, 2, 3}), NimState::PlayerId::FIRST));

    // The hash can't compared to specific values without a predefined ZValueTable, but we can make sure that they vary for
    // different boards and players.

    {
        Board empty({0, 0, 0});
        ZHash z0(empty, NimState::PlayerId::FIRST); // Empty board and first player
        EXPECT_EQ(z0.value(), ZHash::EMPTY);        // Should have a value of 0

        ZHash z1(empty, NimState::PlayerId::SECOND); // Empty board and second player
        EXPECT_NE(z1.value(), ZHash::EMPTY);         // Should not have a value of 0.

        Board board1({1, 2, 3});
        ZHash z2(board1, NimState::PlayerId::FIRST); // Non-empty board and first player
        EXPECT_NE(z2.value(), ZHash::EMPTY);         // Should not have a value of 0.
        EXPECT_NE(z2.value(), z1.value());           // Should not have the value of any other board.

        ZHash z3(board1, NimState::PlayerId::SECOND); // Non-empty board and second player
        EXPECT_NE(z3.value(), ZHash::EMPTY);          // Should not have a value of 0.
        EXPECT_NE(z3.value(), z1.value());            // Should not have the value of any other board.
        EXPECT_NE(z3.value(), z2.value());            // Should not have the value of any other board.
    }

    // Just to be thorough and knowing that every empty heap has a hash value of 0, we should check all permutations except all
    // empty.
    {
        Board boards[] = {Board({0, 0, 1}),
                          Board({0, 1, 0}),
                          Board({1, 0, 0}),
                          Board({0, 1, 1}),
                          Board({1, 0, 1}),
                          Board({1, 1, 0}),
                          Board({1, 1, 1})};

        // Compute the hashes for all boards with the first player.
        std::vector<ZHash> hashes;
        hashes.reserve(std::size(boards));
        std::transform(std::begin(boards),
                       std::end(boards),
                       std::back_inserter(hashes),
                       [](const Board & board) { return ZHash(board, NimState::PlayerId::FIRST); });

        // Make sure that none of the hashes are 0.
        for (auto const & hash : hashes)
        {
            EXPECT_NE(hash.value(), ZHash::EMPTY);
        }

        // Check that all hashes are different.
        EXPECT_FALSE(containsDuplicates(hashes));
    }
}

TEST(ZHash, Value)
{
    // I'll think of a way to test this later. ZHash::value() is used everywhere, so it will be tested indirectly.
}

TEST(ZHash, IsUndefined)
{
    EXPECT_TRUE(ZHash(ZHash::UNDEFINED).isUndefined());
    EXPECT_FALSE(ZHash(ZHash::EMPTY).isUndefined());
    EXPECT_FALSE(ZHash(1).isUndefined());
}

TEST(ZHash, ChangeHeap)
{
    // Since values are random, they cannot be compared to predetermined values.

    // Make a bunch of changes to heaps and then undo them in the same order. All hashes should be different, but the final hash
    // should be the same as the initial one.
    std::tuple<int8_t, int8_t, int8_t> changes[] = {{0, 0, 1}, {0, 1, 2}, {1, 0, 1}, {1, 1, 2}, {2, 0, 1}, {2, 1, 2}};

    ZHash              z(0xB1D28E7F56A3F9C4);
    ZHash              z0 = z; // Save the initial hash value
    std::vector<ZHash> hashes;
    hashes.reserve(std::size(changes) * 2);
    for (const auto & change : changes)
    {
        int i    = std::get<0>(change);
        int from = std::get<1>(change);
        int to   = std::get<2>(change);
        z        = z.changeHeap(i, from, to);
        hashes.push_back(z); // Save the hash after each change
    }
    for (const auto & change : changes)
    {
        int i    = std::get<0>(change);
        int from = std::get<1>(change);
        int to   = std::get<2>(change);
        z        = z.changeHeap(i, from, to);
        hashes.push_back(z); // Save the hash after each change
    }
    ZHash zz = hashes.back(); // Get the last hash, which should be the same as the initial one
    hashes.pop_back();        // Remove the last hash so it is not included in the generic comparisons

    EXPECT_EQ(z0.value(), zz.value());        // The final hash should be the same as the initial one
    EXPECT_FALSE(containsDuplicates(hashes)); // All hashes should be different
    EXPECT_FALSE(std::any_of(hashes.begin(),
                             hashes.end(),
                             [&z0](ZHash const & hash)
                             { return hash.value() == z0.value(); })); // None of the hashes should be the same as the initial one
}

TEST(ZHash, ChangeNextPlayer)
{
    // Since values are random, they cannot be compared to predetermined values.
    ZHash z(0xA3F9C4B1D28E7F56);
    ZHash z0 = z;
    ZHash z1 = z.changeNextPlayer();
    ZHash z2 = z.changeNextPlayer();
    EXPECT_NE(z0.value(), z1.value());
    EXPECT_NE(z1.value(), z2.value());
    EXPECT_EQ(z0.value(), z2.value()); // two changes should return to the original state
}

} // namespace Nim
