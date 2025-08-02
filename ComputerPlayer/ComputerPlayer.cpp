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

static const int TRANSPOSITION_TABLE_SIZE = 4000000; // Enough for 10! states, probably could go higher without issues
static const int MAXIMUM_DEPTH = 10;                  // Determines how good the AI is and how long it takes to respond

ComputerPlayer::ComputerPlayer(NimState::PlayerId playerId, Rules::Variation variation/* = Rules::Variation::DEFAULT*/)
    : Player(playerId, variation)
    , gameTree_(nullptr)
    , staticEvaluator_(nullptr)
    , transpositionTable_(nullptr)
{
    staticEvaluator_    = std::make_shared<NimEvaluator>();
    transpositionTable_ = std::make_shared<GamePlayer::TranspositionTable>(TRANSPOSITION_TABLE_SIZE, MAXIMUM_DEPTH);
    gameTree_ = new GamePlayer::GameTree(transpositionTable_,
                                         staticEvaluator_,
                                         std::bind(&ComputerPlayer::responseGenerator, this, std::placeholders::_1,
                                                   std::placeholders::_2),
                                         MAXIMUM_DEPTH);
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed the random number generator
}

void ComputerPlayer::move(NimState * pState)
{
    assert(pState);
    assert(pState->whoseTurn() == playerId_);
    assert(pState->isGameOver() == false);

    // for now, just generate a random move
    Board const & board = pState->board();
    assert(!board.empty());

    // Generate a random index of a heap
    size_t i = std::rand() % board.size();
    while (board.heap(i) == 0) // Find a heap with objects
    {
        i = std::rand() % board.size();
    }
    // Generate a random number of objects to remove from the heap
    int n = std::rand() % board.heap(i) + 1; // At least one object, at most all objects in the heap
    
    // Apply the move
    pState->move(i, n);

    
//    // Find the best response to the current state
//    auto pCopy     = std::make_shared<NimState>(*pState);
//    gameTree_->findBestResponse(std::static_pointer_cast<GamePlayer::GameState>(pCopy));
//    auto pResponse = std::dynamic_pointer_cast<NimState>(pCopy->response_);
//    assert(pResponse);
//    *pState = *pResponse;
}

std::vector<GamePlayer::GameState *> ComputerPlayer::responseGenerator(GamePlayer::GameState const & state, int depth)
{
    std::vector<GamePlayer::GameState *> responses;
//    NimState const * pNimState = dynamic_cast<NimState const *>(&state);
//    for (int i = 0; i < 9; ++i)
//    {
//        if (pNimState->board().at(i) == Board::Cell::NEITHER)
//        {
//            NimState * pResponse = new NimState(*pNimState);
//            auto [r, c] = Board::toPosition(i);
//            pResponse->move(r, c);
//            responses.push_back(pResponse);
//        }
//    }
    return responses;
}
