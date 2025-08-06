#include "ComputerPlayer.h"

#include "NimEvaluator.h"

#include "Components/Board.h"
#include "Components/Rules.h"
#include "GamePlayer/GameTree.h"
#include "GamePlayer/TranspositionTable.h"
#include "NimState/NimState.h"

#include <algorithm>
#include <cassert>
#include <ctime>
#include <functional>
#include <iterator>
#include <utility>
#include <vector>

static const int TRANSPOSITION_TABLE_SIZE = 100000; // Limits memory usage of the transposition table
static const int MAXIMUM_DEPTH            = 10;     // Determines how good the AI is and how long it takes to respond

ComputerPlayer::ComputerPlayer(NimState::PlayerId playerId, Rules const & rules /*= Rules()*/)
    : Player(playerId, rules)
    , gameTree_(nullptr)
    , staticEvaluator_(nullptr)
    , transpositionTable_(nullptr)
{
    staticEvaluator_    = std::make_shared<NimEvaluator>(rules);
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
    std::vector<int8_t> const &          heaps    = board.heaps();

    if (rules_.variation() == Rules::Variation::SUBTRACT)
    {
        // In the subtraction variation, only one heap is used
        assert(heaps.size() == 1);
        int max = std::min(static_cast<int>(heaps[0]), rules_.removalLimit());
        for (int n = 1; n <= max; ++n)
        {
            NimState * pResponse = new NimState(nimState);
            pResponse->move(0, n);
            responses.push_back(pResponse);
        }
    }
    else
    {
        // Get a list of unique heaps to consider in order to avoid duplicate moves on heaps with the same number of objects
        std::vector<std::pair<int, int>> distinctHeaps;
        for (size_t i = 0; i < heaps.size(); ++i)
        {
            distinctHeaps.emplace_back(heaps[i], static_cast<int>(i)); // (n, i) to make sorting easier
        }
        std::sort(distinctHeaps.begin(), distinctHeaps.end());
        auto last = std::unique(distinctHeaps.begin(),
                                distinctHeaps.end(),
                                [](auto const & a, auto const & b) { return a.first == b.first; });
        distinctHeaps.erase(last, distinctHeaps.end());

        // Generate all possible responses, excluding duplicates
        for (auto const & [size, i] : distinctHeaps)
        {
            if (size > 0) // Only consider heaps with objects
            {
                for (int n = 1; n <= size; ++n) // Remove 1 to all objects in the heap
                {
                    NimState * pResponse = new NimState(nimState);
                    pResponse->move(i, n);
                    responses.push_back(pResponse);
                }
            }
        }
    }
    return responses;
}
