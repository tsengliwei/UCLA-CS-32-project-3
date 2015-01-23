//
//  Board.cpp
//  Project 3
//
//  Created by Li-Wei Tseng on 5/11/14.
//  Copyright (c) 2014 Li-Wei Tseng. All rights reserved.
//

#include "Board.h"
#include "Side.h"

Board::Board(int nHoles, int nInitialBeansPerHole)
:s_nHoles(nHoles)
{
    northSide.push_back(0);
    southSide.push_back(0); //push 0s to the pots
    for(int i = 1; i <= s_nHoles; i++)
    {
        //construct NORTH side
        northSide.push_back(nInitialBeansPerHole);
    }
    
    for(int i = 1; i <= s_nHoles; i++)
    {
        //construct SOUTH side
        southSide.push_back(nInitialBeansPerHole);
    }
}
//Construct a Board with the indicated number of holes per side (not counting the pot) and initial number of beans per hole.

int Board::holes() const
{
    return s_nHoles;
}
//Return the number of holes on a side (not counting the pot).

int Board::beans(Side s, int hole) const
{
    if(s == NORTH && hole <= s_nHoles) //if side and hole are valid, return the values in the indicated spot
    {
        return northSide[hole];
    }
    else if(s == SOUTH && hole <= s_nHoles)
    {
        return southSide[hole];
    }
    else
        return -1; //if side and hole aren invalid, return -1
}
//Return the number of beans in the indicated hole or pot, or −1 if the hole number is invalid.

int Board::beansInPlay(Side s) const
{
    if(s == NORTH) //iterate through the indicated side and count the total number of beans
    {
        int count = 0;
        for(int i = 1; i <= s_nHoles; i++) //i starts from 1 becuase the pot is excluded
        {
            count += northSide[i];
        }
        return count;
    }
    else if(s == SOUTH)
    {
        int count = 0;
        for(int i = 1; i <= s_nHoles; i++)
        {
            count += southSide[i];
        }
        return count;
    }
    else
        return -1;
}
//Return the total number of beans in all the holes on the indicated side, not counting the beans in the pot.

int Board::totalBeans() const
{
    int count = 0;
    
    for(int i = 0; i <= s_nHoles; i++) //i starts from 1 becuase the pot is included
    {
        count += northSide[i];
    }
    
    for(int i = 0; i <= s_nHoles; i++)
    {
        count += southSide[i];
    }
    return count;
}
//Return the total number of beans in the game.

bool Board::sow(Side s, int hole, Side& endSide, int& endHole)
{
    if(hole > s_nHoles || hole <= 0 || beans(s, hole) == 0)
        return false; // If the hole indicated by (s,hole) is empty or invalid or a pot, this function returns false without changing anything.
    
    int count = (s==NORTH) ? northSide[hole] : southSide[hole]; //the count keep tracks of the amount of beans in the indicated hole.
    
    if(s == NORTH) //set the hole being sown to 0 beans
        northSide[hole] = 0;
    else
        southSide[hole] = 0;
    
    endHole = hole;
    endSide = s;
    
    for(; count != 0; count--)
    {
        if(endHole == 0) //if we are in a pot, advance to the opponent's side
        {
            if(endSide == SOUTH)
            {
                endHole = s_nHoles;
            }
            else
            {
                endHole = 1;
            }
            endSide = opponent(endSide);
        }
        
        else if(endHole == s_nHoles && endSide == SOUTH) //if we are at the end hole of the south, advance to south's pot
        {
            if(s == SOUTH)
                endHole = 0;
            else
                endHole = s_nHoles; //if we originally are on the opposite side, skip the hole.
        }
        
        else if(endHole == 1 && endSide == NORTH) // if we are at the end hole of the north, advance to north's pot
        {
            if(s == NORTH)
                endHole = 0;
            else
                endHole = 1; //if we originally are on the opposite side, skip the hole.
        }
        
        else
        {
            endHole += (endSide == NORTH)? -1:1; //in all other cases, decrement if it were north, increment otherwise
        }
        
        if(endSide == NORTH)
            northSide[endHole]++;
        else
            southSide[endHole]++;
    }
    return true;
}
// If the hole indicated by (s,hole) is empty or invalid or a pot, this function returns false without changing anything. Otherwise, it will return true after sowing the beans: the beans are removed from hole (s,hole) and sown counterclockwise, including s's pot if encountered, but skipping s's opponent's pot. The parameters endSide and endHole are set to the side and hole where the last bean was placed. (This function does not make captures or multiple turns; different Kalah variants have different rules about these issues, so dealing with them should not be the responsibility of the Board class.)

bool Board::moveToPot(Side s, int hole, Side potOwner)
{
    if(hole > s_nHoles || hole <= 0)
        return false; // If the indicated hole is invalid or a pot, return false without changing anything.
    //Otherwise, move all the beans in hole (s,hole) into the pot belonging to potOwner and return true.

    int count = 0; //count keep tracks of the amout of beans the indicated hole have
    if(s == NORTH)
    {
        count = northSide[hole];
        northSide[hole] = 0;
        if(potOwner == NORTH)
        {
            northSide[0] += count;
        }
        else
            southSide[0] += count;
    }
    
    else
    {
        count = southSide[hole];
        southSide[hole] = 0;
        if(potOwner == NORTH)
        {
            northSide[0] += count;
        }
        else
            southSide[0] += count;
    }
    
    return true;
}
// If the indicated hole is invalid or a pot, return false without changing anything. Otherwise, move all the beans in hole (s,hole) into the pot belonging to potOwner and return true.

bool Board::setBeans(Side s, int hole, int beans)
{
    if(hole > s_nHoles || hole < 0)
        return false;
    if(s == NORTH)
    {
        northSide[hole] = beans;
    }
    else
        southSide[hole] = beans;
    return true;
}
//   If the indicated hole is invalid or beans is negative, this function returns false without changing anything. Otherwise, it will return true after setting the number of beans in the indicated hole or pot to the value of the third parameter. (This may change what beansInPlay and totalBeans return if they are called later.) This function exists solely so that we and you can more easily test your program: None of your code that implements the member functions of any class is allowed to call this function directly or indirectly. (We'll show an example of its use below.)

