#include "HumanPlayer.h"

#include "Components/Board.h"
#include "NimState/NimState.h"
#include "Components/Rules.h"

#include <iostream>
#include <iomanip> // for std::setw

HumanPlayer::HumanPlayer(NimState::PlayerId playerId, Rules::Variation variation/* = Rules::Variation::DEFAULT*/)
    : Player(playerId, variation)
{
}

void HumanPlayer::move(NimState * pState)
{
    Board const& board = pState->board();
    
    char heapLetter;
    int n;
    int i;
    
    // Get heap selection from user
    do {
        std::cout << "Select heap (A-" << char('A' + board.size() - 1) << ") and the number to remove: ";
        std::cin >> heapLetter >> n;
        
        // Convert to uppercase if lowercase
        heapLetter = std::toupper(heapLetter);
        i = heapLetter - 'A';
        
        if (i < 0 || i >= static_cast<int>(board.size())) {
            std::cout << "Invalid heap selection. Please choose a heap from A to " 
                      << char('A' + board.size() - 1) << "." << std::endl;
        } else if (board.heap(i) == 0) {
            std::cout << "Heap " << heapLetter << " is empty. Please choose a non-empty heap." << std::endl;
        }
        else if (n < 1 || n > board.heap(i)) {
            std::cout << "Invalid number. Please enter a number between 1 and " 
                      << board.heap(i) << "." << std::endl;
        }
    } while (!(0 <= i && i < static_cast<int>(board.size())) || !(1 <= n && n <= board.heap(i)));
    
    // Apply the move
    pState->move(i, n);
    
    std::cout << "You removed " << n << " object(s) from heap " << heapLetter << "." << std::endl;
}
