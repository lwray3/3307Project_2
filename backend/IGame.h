#pragma once
#include <iostream>
#include <vector>
#include "Card.h"

class IGame {

public: 
    virtual ~IGame() = default;
    virtual int calcScore(std::vector<Card> hand) = 0;
    virtual void explain_rules() = 0;
    virtual void play() = 0;

}; 