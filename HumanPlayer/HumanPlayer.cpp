#include "HumanPlayer.h"

#include "Components/Board.h"
#include "Components/Rules.h"
#include "NimState/NimState.h"

#include <iomanip>
#include <iostream>

HumanPlayer::HumanPlayer(NimState::PlayerId playerId, Rules const & rules)
    : Player(playerId, rules)
{
}

void HumanPlayer::move(NimState * pState)
{
    Board const & board = pState->board();

    char heapLetter = 'A';
    int  n;
    int  i;

    // Get heap selection from user
    do
    {
        if (rules_.variation() == Rules::Variation::SUBTRACT)
        {
            std::cout << "Enter the number of objects to remove: " << std::endl;
            i = 0; // Only one heap in subtraction variation
            std::cin >> n;
            if (n < 1 || n > rules_.removalLimit())
            {
                std::cout << "Invalid number. Enter a number between 1 and " << rules_.removalLimit() << "." << std::endl;
            }
            else if (n > board.heap(i))
            {
                std::cout << "You can remove up to " << board.heap(i) << "objects." << std::endl;
            }
        }
        else
        {
            std::cout << "Select heap (A-" << char('A' + board.size() - 1) << ") and the number to remove: ";
            std::cin >> heapLetter >> n;

            // Convert to uppercase if lowercase
            heapLetter = std::toupper(heapLetter);
            i          = heapLetter - 'A';

            if (i < 0 || i >= static_cast<int>(board.size()))
            {
                std::cout << "Invalid heap selection. Please choose a heap from A to " << char('A' + board.size() - 1) << "."
                          << std::endl;
            }
            else if (board.heap(i) == 0)
            {
                std::cout << "Heap " << heapLetter << " is empty. Please choose a non-empty heap." << std::endl;
            }
            else if (n < 1 || n > board.heap(i))
            {
                std::cout << "Invalid number. Enter a number between 1 and " << board.heap(i) << "." << std::endl;
            }
        }
    } while (!(0 <= i && i < static_cast<int>(board.size())) || !(1 <= n && n <= board.heap(i)));

    // Apply the move
    pState->move(i, n);

    if (rules_.variation() == Rules::Variation::SUBTRACT)
    {
        std::cout << "You removed " << n << "." << std::endl;
    }
    else
    {
        std::cout << "You removed " << n << " from heap " << heapLetter << "." << std::endl;
    }
}
