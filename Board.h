//
//  Board.h
//  Project 3
//
//  Created by Li-Wei Tseng on 5/11/14.
//  Copyright (c) 2014 Li-Wei Tseng. All rights reserved.
//

#ifndef __Project_3__Board__
#define __Project_3__Board__

#include <iostream>
#include <vector>
#include "Side.h"

class Board
{
public:
    
    Board(int nHoles, int nInitialBeansPerHole);
    int holes() const;
    int beans(Side s, int hole) const;
    int beansInPlay(Side s) const;
    int totalBeans() const;
    bool sow(Side s, int hole, Side& endSide, int& endHole);
    bool moveToPot(Side s, int hole, Side potOwner);
    bool setBeans(Side s, int hole, int beans);
    
private:
    int s_nHoles;
    std::vector<int> northSide;
    std::vector<int> southSide;
};

#endif /* defined(__Project_3__Board__) */
