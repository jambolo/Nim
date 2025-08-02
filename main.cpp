#include "NimState/NimState.h"
#include "HumanPlayer/HumanPlayer.h"
#include "ComputerPlayer/ComputerPlayer.h"
#include "Components/Board.h"
#include "Components/Rules.h"

#include <CLI/CLI.hpp>

#include <cassert>
#include <iostream>

using namespace GamePlayer;

static void displayBoard(const Board& board);

int main(int argc, char * argv[])
{
    bool humanGoesFirst = true;     // Human player goes first by default
    Rules::Variation variation = Rules::Variation::MISERE; // Default variation is misère
    std::vector<int> initialConfiguration{ 1, 3, 5, 7 }; // Default initial configuration

    {
        CLI::App cli;
        bool                first  = false;
        bool                second = false;
        bool                normal = false;
        std::vector<int>    initial;

        auto *   order  = cli.add_option_group("Order of play", "Choose who goes first");
        order->add_flag("--first, -f", first, "You go first. (default)");
        order->add_flag("--second, -s", second, "The computer goes first.");
        order->require_option(0, 1);
        cli.add_flag("--normal, -n", normal, "Play the normal variation game of Nim. Default is the misère variation.");
        cli.add_option("--initial, -i", initial, "Initial configuration (list of integers, 1 - 99)")
            ->expected(1, (int)Board::MAX_HEAPS)
            ->check(CLI::Range(1, (int)Board::MAX_OBJECTS));
        CLI11_PARSE(cli, argc, argv);

        if (first)
        {
            humanGoesFirst = true;
        }
        else if (second)
        {
            humanGoesFirst = false;
        }

        if (normal)
        {
            variation = Rules::Variation::NORMAL;
        }

        if (!initial.empty())
        {
            initialConfiguration = initial;
        }
    }

    std::cout << "Starting Nim game with the following settings:" << std::endl;
    std::cout << "  Human goes first: " << (humanGoesFirst ? "Yes" : "No") << std::endl;
    std::cout << "  Variation: " << (variation == Rules::Variation::NORMAL ? "Normal" : "Misère") << std::endl;
    std::cout << std::endl;

    Board initialBoard(initialConfiguration);
    NimState state(initialBoard, variation);
    HumanPlayer    human(humanGoesFirst ? GameState::PlayerId::FIRST : GameState::PlayerId::SECOND, variation);
    ComputerPlayer computer(humanGoesFirst ? GameState::PlayerId::SECOND : GameState::PlayerId::FIRST, variation);

    while (!state.isGameOver())
    {
        displayBoard(state.board());
        if (state.whoseTurn() == human.playerId())
        {
            human.move(&state);
        }
        else
        {
            computer.move(&state);
            std::cout << "The computer removed " << state.lastMove()->second << " from heap " 
                << char('A' + state.lastMove()->first) << "." << std::endl;
        }
    }
    // Game is over, display the final board state
    displayBoard(state.board());

    assert(state.winner().has_value());
    if (state.winner().value() == human.playerId())
    {
        std::cout << "Congratulations! You win!" << std::endl;
    }
    else
    {
        std::cout << "The computer wins. Better luck next time!" << std::endl;
    }

    return 0;
}

static void displayBoard(const Board & board)
{
    // Display the current state of the board
    for (size_t i = 0; i < board.size(); ++i)
    {
        std::cout << std::setw(2) << board.heap(i) << "  ";
    }
    std::cout << std::endl;
    for (size_t i = 0; i < board.size(); ++i)
    {
        std::cout << "--- ";
    }
    std::cout << std::endl;

    for (size_t i = 0; i < board.size(); ++i)
    {
        std::cout << " " << char('A' + i) << "  ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
}