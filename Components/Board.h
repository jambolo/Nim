#pragma once

#include <vector>

class Board
{
public:
    static int const MAX_HEAPS      = 26;   // Maximum number of heaps allowed
    static int const MAX_OBJECTS    = 99;   // Maximum number of objects in a heap

    // Constructor
    Board(std::vector<int> heaps);

    // Returns the contents of the board as a vector
    std::vector<int> const & heaps() const { return heaps_; }

    // Returns the number of heaps on the board
    size_t size() const { return heaps_.size(); }

    // Returns true if the board is empty
    bool empty() const;

    // Returns the number of objects in the given heap
    int heap(int i) const;

    // Removes `n` objects from heap `i`. 'n' must be > 0. Returns the new number of objects in the heap.
    int remove(int i, int n);

private:
    std::vector<int> heaps_;
};

inline bool operator ==(Board const & lhs, Board const & rhs)
{
    return lhs.heaps() == rhs.heaps();
}
