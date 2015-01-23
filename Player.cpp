//
//  Player.cpp
//  Project 3
//
//  Created by Li-Wei Tseng on 5/11/14.
//  Copyright (c) 2014 Li-Wei Tseng. All rights reserved.
//

#include "Player.h"
#include "Board.h"
#include <iostream>
#include <string>
#include <limits>

using namespace std;

//========================================================================
// Timer t;                 // create a timer
// t.start();               // start the timer
// double d = t.elapsed();  // milliseconds since timer was last started
//========================================================================

#if __cplusplus == 201103L  // C++11

#include <chrono>

class Timer
{
public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const
    {
        std::chrono::duration<double,std::milli> diff =
        std::chrono::high_resolution_clock::now() - m_time;
        return diff.count();
    }
private:
    std::chrono::high_resolution_clock::time_point m_time;
};

#elif defined(_MSC_VER)  // not C++11, but Windows

#include <windows.h>

class Timer
{
public:
    Timer()
    {
        QueryPerformanceFrequency(&ticksPerSecond);
        start();
    }
    void start()
    {
        QueryPerformanceCounter(&m_time);
    }
    double elapsed() const
    {
        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        return (1000.0 * (now.QuadPart - m_time.QuadPart)) / ticksPerSecond.QuadPart;
    }
private:
    LARGE_INTEGER m_time;
    LARGE_INTEGER ticksPerSecond;
};

#else // not C++11 or Windows, so C++98

#include <ctime>

class Timer
{
public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::clock();
    }
    double elapsed() const
    {
        return (1000.0 * (std::clock() - m_time)) / CLOCKS_PER_SEC;
    }
private:
    std::clock_t m_time;
};

#endif


Timer timer;

int SmartPlayer::eval(const Board& b, Side s) const
{
    int value;
    Board tempB(b);
    
    if((s == NORTH && tempB.beans(NORTH, 1) - 1 == 0) || (s == SOUTH && tempB.beans(SOUTH, b.holes()) - 1 == 0))
    {
        for(int i = 1; i <= tempB.holes(); i++)
        {
            tempB.moveToPot(s, i, s);
            tempB.moveToPot(opponent(s), i, opponent(s));
        }
        if(tempB.beans(s, 0) > tempB.beans(opponent(s), 0))
            value = 10000; //if one move to win, set value to a large num
        else
            value = -10000; //if one move to lose, set value to a low num
    }
    
    Side endSide;
    int endHole;
    
    for(int i = 1; i < tempB.holes(); i++)
    {
        Board tempB2(b);
        if(!tempB2.sow(s, i, endSide, endHole))
            continue;
        if(endHole == 0 && s == endSide)
            value = 1000; //if one move to get another turn, set value to a large num
    }
    
    for(int i = 1; i < tempB.holes(); i++)
    {
        Board tempB2(b);
        if(!tempB2.sow(s, i, endSide, endHole))
            continue;
        
        if(endHole != 0 && endSide == s && tempB.beans(s, endHole) - 1 == 0 && tempB.beans(opponent(s), endHole) != 0)
            value = 100; // if one move to capture, set value to a large num
    }
    
    
           
    
    value = b.beans(SOUTH, 0) - b.beans(NORTH, 0);// in regular case, set the value to be the difference between two pots
           
    return value;
}

void SmartPlayer::minimax(int& bestHole, int& value, const Board& b, Side s, int searchDepth) const
{
    value = (s == NORTH)? 1000000000:-1000000000;//if the side is south, set value to negative infinity, while set value to positive infinity if the side is south
    
    if(b.beansInPlay(SOUTH) == 0 || b.beansInPlay(NORTH) == 0)
    {
        //if no move for player exists
        bestHole = -1;
        if(b.beans(SOUTH, 0) > b.beans(NORTH, 0))
            value = 1000000000; //set to positive inifinity if south wins
        if(b.beans(SOUTH, 0) > b.beans(NORTH, 0))
            value = -1000000000; //set to negative infinity if north win
        else
            value = 0; //set value to 0 if there's a tie
        return;
    }
    
    
    if(timer.elapsed() > 4900)
    {
        //cerr << "time limit reached" << endl;
        value = eval(b, s);
        return;
    }
    
    
    if(searchDepth <= 0) //criterion
    {
        //cerr << "searchDepth reached" << endl;
        bestHole = -1;
        value = eval(b, s);
        return;
    }
    
    Side endSide;
    
    
    
    for(int hole = 1; hole <= b.holes(); hole++)
    {
        int endHole;
        
        Board tempB(b);// a temp Board: without moving for real
        if(!tempB.sow(s, hole, endSide, endHole))//if it's sowable, make the move
            continue;
        
        
        if(endHole != 0 && endSide == s && tempB.beans(s, endHole) - 1 == 0 && tempB.beans(opponent(s), endHole) != 0)
        {
            //If it was placed in one of the player's own holes that was empty just a moment before, and if the opponent's hole directly opposite from that hole is not empty, capture the beans.
            tempB.moveToPot(s, endHole, s);
            tempB.moveToPot(opponent(s), endHole, s);
            value+=2;
        }
        
        while(endHole == 0 && s == endSide)
        {
            //if the bean ends at the the player's pot, it gets another turn
            int n;
            do
            {
                n = (rand() % b.holes()) + 1;
            }while(tempB.beans(s, n) == 0 && tempB.beansInPlay(s) != 0 && tempB.beansInPlay(opponent(s)) != 0);
            if(!tempB.sow(s, n, endSide, endHole))
                break;
            value++;
        }
        
        
        if(tempB.beansInPlay(SOUTH) == 0 || tempB.beansInPlay(NORTH) == 0)
        {
            for(int i = 1; i <= b.holes(); i++)
            {
                tempB.moveToPot(s, i, s);
                tempB.moveToPot(opponent(s), i, opponent(s));
            }//move the remaining beans to the pots when the game is over
        }
        
        /////////////////////test opponent's best move////////////////
        int opponentHole, opponentValue;
        
        minimax(opponentHole, opponentValue, tempB, opponent(s), searchDepth - 1); //call oponent's minimax to know its next move
        
        /////////unmake the move//////////////////
        
        //Board originalB(b); //compare the original board to choose the best move
        
        //////////////////////////////////////////
        
        if(s == SOUTH && value <= opponentValue)//if v2 is better for the player than best seen so far. South will choose the move with large value
        {
            bestHole = hole;
            value = opponentValue;
        }
        
        if(s == NORTH && value >= opponentValue)//if v2 is better for the player than best seen so far. North will choose the move with small value
        {
            bestHole = hole;
            value = opponentValue;
        }
        
    }
    return;
}







int HumanPlayer::chooseMove(const Board& b, Side s) const
{
    int n;
    
    if(b.beansInPlay(s) == 0)
        return -1; //if beans in both sides' holes are empty, return -1
    do
    {
        cout << "Select a hole from 1 to " << b.holes() << ": ";
        cin >> n;
        
    }while(n > b.holes() || n < 1 || b.beans(s, n) == 0);
    //keeps prompting until the player enters a valid hole
    
    return n;
}

int BadPlayer::chooseMove(const Board& b, Side s) const
{
    int n;
    
    if(b.beansInPlay(SOUTH) == 0 || b.beansInPlay(NORTH) == 0)
        return -1; //if beans in its sides' holes are empty, return -1
    
    do
    {
        n = (rand() % b.holes()) + 1;
    }while(b.beans(s, n) == 0);
    //assign a random valid hole. If the hole is empty, assign again.
    
    return n;
}

int SmartPlayer::chooseMove(const Board& b, Side s) const
{
    int bestHole, value;
    
    if(b.beansInPlay(SOUTH) == 0 || b.beansInPlay(NORTH) == 0)
        return -1; //if beans in both sides' holes are empty, return -1
    
   do
    {
        bestHole = (rand() % b.holes()) + 1;
    }while(b.beans(s, bestHole) == 0);

    //assign a random valid hole. If the hole is empty, assign again.
    int searchDepth = 10;

    Timer timer;
    timer.start();
    minimax(bestHole, value, b, s, searchDepth);
    
    return bestHole;
}

