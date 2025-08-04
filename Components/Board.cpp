#include "Board.h"

#include <algorithm>
#include <cassert>
#include <numeric>
#include <vector>

// Constructor
Board::Board(std::vector<int8_t> heaps)
    : heaps_(std::move(heaps))
{
    assert(0 < heaps_.size() && heaps_.size() <= MAX_HEAPS); // Ensure the number of heaps does not exceed the maximum allowed
    for (int n : heaps_)
    {
        assert(0 <= n && n <= MAX_OBJECTS); // Ensure each heap has a valid number of objects
    }
}

// Returns true if all the heaps are empty
bool Board::empty() const
{
    return std::all_of(heaps_.begin(), heaps_.end(), [](int n) { return n == 0; });
}

int Board::heap(int i) const
{
    assert(0 <= i && i < heaps_.size()); // Ensure the index is within bounds
    return heaps_[i];
}

int Board::nimSum() const
{
    return std::reduce(heaps_.begin(), heaps_.end(), 0, [](int sum, int heap) { return sum ^ heap; });
}

int Board::count() const
{
    return std::count_if(heaps_.begin(), heaps_.end(), [](int n) { return n > 0; });
}

int Board::remove(int i, int n)
{
    assert(0 <= i && i < heaps_.size()); // Ensure the index is within bounds
    assert(0 < n && n <= heaps_[i]);     // Ensure the number of objects to remove is valid
    heaps_[i] -= n;
    return heaps_[i];
}
