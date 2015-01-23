//
//  Side.h
//  Project 3
//
//  Created by Li-Wei Tseng on 5/11/14.
//  Copyright (c) 2014 Li-Wei Tseng. All rights reserved.
//

#ifndef __Project_3__Side__
#define __Project_3__Side__

#include <iostream>

class Player;
class Board;

enum Side { NORTH, SOUTH };

inline
Side opponent(Side s)
{
    return Side(1 - s);
}
void f(bool& startWrappingItUp);

#endif /* defined(__Project_3__Side__) */
