#pragma once

#include "Components/Player.h"
#include "Components/Rules.h"

class HumanPlayer : public Player
{
public:
    // Constructor
    explicit HumanPlayer(NimState::PlayerId playerId, Rules const & rules);

    // Gets a move from the human and applies it to the game state. Overrides Player::move().
    virtual void move(NimState * pState) override;
};
