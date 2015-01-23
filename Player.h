//
//  Player.h
//  Project 3
//
//  Created by Li-Wei Tseng on 5/11/14.
//  Copyright (c) 2014 Li-Wei Tseng. All rights reserved.
//

#ifndef __Project_3__Player__
#define __Project_3__Player__

#include <iostream>
#include <string>
#include "Side.h"

class Board;

class Player
{
public:
    Player(std::string name){m_name = name;}
    std::string name() const{return m_name;}
    virtual bool isInteractive() const {return false;}
    virtual int chooseMove(const Board& b, Side s) const = 0;
    
    virtual ~Player(){}
       // Since this class is designed as a base class, it should have a virtual destructor.
private:
    std::string m_name;
};


//A HumanPlayer chooses its move by prompting a person running the program for a move (reprompting if necessary until the person enters a valid move), and returning that choice.
class HumanPlayer:public Player
{
public:
    HumanPlayer(std::string name): Player(name){}
    virtual bool isInteractive() const {return true;}
    virtual int chooseMove(const Board& b, Side s) const;
    
    ~HumanPlayer(){}
};


//A BadPlayer is a computer player that chooses an arbitrary valid move and returns that choice. "Arbitrary" can be what you like: leftmost, nearest to pot, fewest beans, random, etc.. The point of this class is to have an easy-to-implement class that at least plays legally.

class BadPlayer:public Player
{
public:
    BadPlayer(std::string name): Player(name){}
    virtual int chooseMove(const Board& b, Side s) const;

    ~BadPlayer(){}
};



//Here's your chance to shine. A SmartPlayer chooses a valid move and returns it. For any game played on a board of up to six holes, with up to four initial beans per hole, SmartPlayer::chooseMove must return its choice in no more than [SOME TIME LIMIT THAT WE WILL REPLACE THIS TEXT WITH LATER; PERHAPS ABOUT FIVE SECONDS]. (We'll give you a way of determining the time soon; until then, you can meet this requirement by limiting the depth of your game tree search to a limit you determine experimentally.) SmartPlayer::chooseMove will be worth about 15% of the points for this project.

class SmartPlayer:public Player
{
public:
    SmartPlayer(std::string name): Player(name){}
    virtual int chooseMove(const Board& b, Side s) const;

    ~SmartPlayer(){};
private:
    void minimax(int& bestHole, int& value, const Board& b, Side s, int searchDepth) const;
    int eval(const Board& b, Side s) const;
};

#endif /* defined(__Project_3__Player__) */
