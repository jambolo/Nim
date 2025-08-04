#include "ComputerPlayer.h"

#include "NimEvaluator.h"

#include "Components/Board.h"
#include "Components/Rules.h"
#include "GamePlayer/GameTree.h"
#include "GamePlayer/TranspositionTable.h"
#include "NimState/NimState.h"

#include <cassert>
#include <ctime>
#include <functional>
#include <vector>

static const int TRANSPOSITION_TABLE_SIZE = 100000; // Limits memory usage of the transposition table
static const int MAXIMUM_DEPTH            = 10;     // Determines how good the AI is and how long it takes to respond

ComputerPlayer::ComputerPlayer(NimState::PlayerId playerId, Rules::Variation variation /* = Rules::Variation::DEFAULT*/)
    : Player(playerId, variation)
    , gameTree_(nullptr)
    , staticEvaluator_(nullptr)
    , transpositionTable_(nullptr)
{
    staticEvaluator_    = std::make_shared<NimEvaluator>(variation);
    transpositionTable_ = std::make_shared<GamePlayer::TranspositionTable>(TRANSPOSITION_TABLE_SIZE, MAXIMUM_DEPTH);
    gameTree_           = new GamePlayer::GameTree(transpositionTable_,
                                         staticEvaluator_,
                                         std::bind(&ComputerPlayer::responseGenerator,
                                                   this,
                                                   std::placeholders::_1,
                                                   std::placeholders::_2),
                                         MAXIMUM_DEPTH);
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed the random number generator
}

void ComputerPlayer::move(NimState * pState)
{
    assert(pState);
    assert(pState->whoseTurn() == playerId_);
    assert(pState->isGameOver() == false);

    // Find the best response to the current state
    auto pCopy = std::make_shared<NimState>(*pState);
    gameTree_->findBestResponse(std::static_pointer_cast<GamePlayer::GameState>(pCopy));
    auto pResponse = std::dynamic_pointer_cast<NimState>(pCopy->response_);
    assert(pResponse);
    *pState = *pResponse;
}

std::vector<GamePlayer::GameState *> ComputerPlayer::responseGenerator(GamePlayer::GameState const & state, int depth)
{
    std::vector<GamePlayer::GameState *> responses;
    auto const &                         nimState = dynamic_cast<NimState const &>(state);
    Board const &                        board    = nimState.board();

    // Generate all possible responses from the current state
    for (int i = 0; i < board.size(); ++i)
    {
        if (board.heap(i) > 0) // Only consider heaps with objects
        {
            for (int n = 1; n <= board.heap(i); ++n) // Remove 1 to all objects in the heap
            {
                NimState * pResponse = new NimState(nimState);
                pResponse->move(i, n);
                responses.push_back(pResponse);
            }
        }
    }
    return responses;
}
