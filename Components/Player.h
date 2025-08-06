#pragma once

#include "Components/Rules.h"
#include "NimState/NimState.h"

// Abstract base class for players in a tic-tac-toe game.
class Player
{
public:
    Player(NimState::PlayerId playerId, Rules rules)
        : playerId_(playerId)
        , rules_(std::move(rules))
    {
    }

    virtual ~Player() = default;

    // Make a move on the given game state. This method must be overridden.
    virtual void move(NimState * pState) = 0;

    // Get the player's ID.
    NimState::PlayerId playerId() const { return playerId_; }

protected:
    NimState::PlayerId playerId_;  // The player's ID
    Rules              rules_;     // The rules for the game being played
};
