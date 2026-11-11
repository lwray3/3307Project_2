
#pragma once

#include <vector>
#include "Card.h"

class IGame {
public:
    virtual ~IGame() = default;
    virtual void startRound() = 0; //ensure all the IGame classes have startRound
    virtual void explain_rules() const = 0; //and explain_rules
    virtual void reset() = 0; //and the reset method
    virtual bool isRoundOver() const = 0; //and isRoundOver
};
