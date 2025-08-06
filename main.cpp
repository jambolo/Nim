#include "Components/Board.h"
#include "Components/Rules.h"
#include "ComputerPlayer/ComputerPlayer.h"
#include "HumanPlayer/HumanPlayer.h"
#include "NimState/NimState.h"

#include <CLI/CLI.hpp>

#include <cassert>
#include <iostream>

using namespace GamePlayer;

static void displayBoard(const Board & board, Rules const & rules);

int main(int argc, char * argv[])
{
    bool                humanGoesFirst = true; // Human player goes first by default
    std::vector<int8_t> initialConfiguration;
    Rules               rules;

    {
        CLI::App            cli;
        bool                first       = true;
        bool                second      = false;
        bool                misere      = true;
        bool                normal      = false;
        bool                subtraction = false;
        std::vector<int8_t> initial;

        auto * order = cli.add_option_group("Order of play", "Choose who goes first");
        order->add_flag("--first, -f", first, "You go first. (default)");
        order->add_flag("--second, -s", second, "The computer goes first.");
        order->require_option(0, 1);

        auto * variations = cli.add_option_group("Game variation", "Choose the variation of Nim to play");
        variations->add_flag("--misere", misere, "")
            ->description(
                "(default) In the misère variation, you win by forcing the other player to take the last object. The default "
                "setup for this variation is 1 3 5 7 9.");
        variations->add_flag("--normal", normal, "")
            ->description("In the normal variation, you win by taking the last object. The default setup for this variation is "
                          "1 3 5 7 9.");
        variations->add_flag("--subtraction", subtraction, "")
            ->description("In the subtraction variation, you can remove a limited number of objects from a single heap on your "
                          "turn. The player who removes the last object wins. The default setup is 21 4 and can be changed with "
                          "--initial.");

        variations->require_option(0, 1);

        cli.add_option("--initial, -i", initial, "Initial configuration")
            ->description("For the misère and normal variations, a list of 1 to 5 numbers with values between 1 and 9 is "
                          "provided. These values describe the number of objects in each heap. For the subtraction variation, "
                          "the size of the heap followed the maximum number of objects that can be removed is provided.");

        cli.description("Play a game of Nim against the computer.");
        cli.callback(
            [&]()
            {
                if (subtraction)
                {
                    if (!initial.empty() && initial.size() != 2)
                    {
                        throw CLI::ValidationError("The correct setup for the subtraction variation is '--initial N k'.");
                    }
                    if (initial.size() == 2)
                    {
                        if (initial[0] < 1 || initial[0] > Board::MAX_OBJECTS)
                        {
                            std::string message = "The number of objects in a heap must be between 1 and " +
                                                  std::to_string(Board::MAX_OBJECTS) + ".";
                            throw CLI::ValidationError(message);
                        }
                        if (initial[1] < 1 || initial[1] >= initial[0])
                        {
                            std::string message = "The maximum number of objects that can be removed must be between 1 and " +
                                                  std::to_string(initial[0]) + ".";
                            throw CLI::ValidationError(message);
                        }
                    }
                }
                else
                {
                    if (initial.size() > Board::MAX_HEAPS)
                    {
                        std::string message = "The maximum number of heaps is " + std::to_string(Board::MAX_HEAPS) + ".";
                        throw CLI::ValidationError(message);
                    }
                    for (auto n : initial)
                    {
                        if (n < 1 || n > Board::MAX_OBJECTS)
                        {
                            std::string message = "The number of objects in each heap must be between 1 and " +
                                                  std::to_string(Board::MAX_OBJECTS) + ".";
                            throw CLI::ValidationError(message);
                        }
                    }
                }
            });
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
            rules                = Rules(Rules::Variation::NORMAL);
            initialConfiguration = initial.empty() ? std::vector<int8_t>{1, 3, 5, 7, 9} : initial;
        }
        else if (subtraction)
        {
            rules                = Rules(Rules::Variation::SUBTRACT, initial.empty() ? 4 : initial[1]);
            initialConfiguration = initial.empty() ? std::vector<int8_t>{21} : std::vector<int8_t>{initial[0]};
        }
        else // if (misere)
        {
            rules                = Rules(Rules::Variation::MISERE);
            initialConfiguration = initial.empty() ? std::vector<int8_t>{1, 3, 5, 7, 9} : initial;
        }
    }

    std::cout << std::endl;

    Board          initialBoard(initialConfiguration);
    NimState       state(initialBoard, rules);
    HumanPlayer    human(humanGoesFirst ? GameState::PlayerId::FIRST : GameState::PlayerId::SECOND, rules);
    ComputerPlayer computer(humanGoesFirst ? GameState::PlayerId::SECOND : GameState::PlayerId::FIRST, rules);

    while (!state.isGameOver())
    {
        displayBoard(state.board(), rules);
        if (state.whoseTurn() == human.playerId())
        {
            human.move(&state);
        }
        else
        {
            computer.move(&state);
            assert(state.lastMove().has_value());
            NimState::Move move = state.lastMove().value();
            if (rules.variation() == Rules::Variation::SUBTRACT)
            {
                std::cout << "The computer removed " << static_cast<int>(move.n) << std::endl;
            }
            else
            {
                std::cout << "The computer removed " << static_cast<int>(move.n) << " from heap " << char('A' + move.i) << "."
                          << std::endl;
            }
        }
    }
    // Game is over, display the final board state
    displayBoard(state.board(), rules);

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

static void displayBoard(const Board & board, Rules const & rules)
{
    if (rules.variation() == Rules::Variation::SUBTRACT)
    {
        std::cout << "Remaining: " << board.heap(0) << std::endl;
    }
    else
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
    }
    std::cout << std::endl;
}
