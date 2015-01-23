//
//  Game.h
//  Project 3
//
//  Created by Li-Wei Tseng on 5/11/14.
//  Copyright (c) 2014 Li-Wei Tseng. All rights reserved.
//

#ifndef __Project_3__Game__
#define __Project_3__Game__

#include <iostream>
#include "Side.h"
#include "Board.h"

class Player;

class Game
{
public:
    Game(const Board& b, Player* south, Player* north);
    void display() const;
    void status(bool& over, bool& hasWinner, Side& winner) const;
    bool move();
    void play();
    int beans(Side s, int hole) const;
    
private:
    Board m_b;
    Player* m_south;
    Player* m_north;
    Side m_turn;
};

#endif /* defined(__Project_3__Game__) */
