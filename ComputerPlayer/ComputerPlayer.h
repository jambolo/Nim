#pragma once

#include "Components/Player.h"
#include "Components/Rules.h"
#include "NimState/NimState.h"

#include <memory>
#include <vector>

namespace GamePlayer
{
class GameTree;
class StaticEvaluator;
class TranspositionTable;
}

class ComputerPlayer : public Player
{
public:
    // Constructor
    explicit ComputerPlayer(NimState::PlayerId playerId, Rules::Variation variation = Rules::Variation::DEFAULT);

    // Makes a move on the game state. Overrides Player::move().
    void ComputerPlayer::move(NimState * pState) override;

private:

    GamePlayer::GameTree * gameTree_;                                    // Game tree for searching responses
    std::shared_ptr<GamePlayer::StaticEvaluator>    staticEvaluator_;    // Static evaluator for the game tree
    std::shared_ptr<GamePlayer::TranspositionTable> transpositionTable_; // Transposition table for the game tree

    std::vector<GamePlayer::GameState *> responseGenerator(GamePlayer::GameState const & state, int depth);
};
