
#pragma once

#include <vector>
#include "Card.h"

class IGame {
public:
    virtual ~IGame() = default;

    virtual void startRound() = 0;

    virtual void explain_rules() const = 0;

    virtual void reset() = 0;

    virtual bool isRoundOver() const = 0;
};
