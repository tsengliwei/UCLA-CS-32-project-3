//
//  Main.cpp
//  Project 3
//
//  Created by Li-Wei Tseng on 5/11/14.
//  Copyright (c) 2014 Li-Wei Tseng. All rights reserved.
//
#include "Game.h"
#include "Player.h"
#include "Board.h"
#include "Side.h"
#include <iostream>
#include <cassert>
using namespace std;


void doBoardTests()
{
    Board b(3,2);
    assert(b.holes() == 3  &&  b.totalBeans() == 12  &&
           b.beans(SOUTH, 0) == 0  &&  b.beansInPlay(SOUTH) == 6);
    b.setBeans(SOUTH, 1, 1);
    b.moveToPot(SOUTH, 2, SOUTH);
    assert(b.totalBeans() == 11  &&  b.beans(SOUTH, 1) == 1  &&
           b.beans(SOUTH, 2) == 0  &&  b.beans(SOUTH, 0) == 2  &&
           b.beansInPlay(SOUTH) == 3);
    Side es;
    int eh;
    b.sow(SOUTH, 3, es, eh);
    assert(es == NORTH  &&  eh == 3  &&  b.beans(SOUTH, 3) == 0  &&
           b.beans(NORTH, 3) == 3  &&  b.beans(SOUTH, 0) == 3  &&
           b.beansInPlay(SOUTH) == 1  &&  b.beansInPlay(NORTH) == 7);
    
    b.setBeans(NORTH, 0, 2);
    b.setBeans(NORTH, 1, 3);
    b.setBeans(NORTH, 2, 3);
    b.setBeans(NORTH, 3, 0);
    b.setBeans(SOUTH, 0, 1);
    b.setBeans(SOUTH, 1, 0);
    b.setBeans(SOUTH, 2, 3);
    b.setBeans(SOUTH, 3, 0);
    
    assert(!b.sow(SOUTH, 0, es, eh) && !b.sow(NORTH, 0, es, eh));//can't sow hole
    assert(!b.sow(SOUTH, 1, es, eh) && !b.sow(SOUTH, 3, es, eh) && !b.sow(NORTH, 3, es, eh));//can't sow empty hole
    
    assert(b.sow(SOUTH, 2, es, eh));
    assert(es == NORTH && eh == 3);
    assert(b.beans(SOUTH, 2) == 0 && b.beans(SOUTH, 3) == 1 && b.beans(SOUTH, 0) == 2 && b.beans(NORTH, 3) == 1); //regular sow on south
    
    
    assert(b.sow(NORTH, 2, es, eh));
    assert(es == SOUTH && eh == 1);
    assert(b.beans(NORTH, 1) == 4 && b.beans(NORTH, 0) == 3 && b.beans(SOUTH, 1) == 1 && b.beans(NORTH, 2) == 0); //regular sow on north
    
    assert(b.beansInPlay(SOUTH) == 2 && b.beansInPlay(NORTH) == 5); //test beansInPlay
    assert(!b.moveToPot(SOUTH, 0, SOUTH) && !b.moveToPot(SOUTH, -1, SOUTH) && !b.moveToPot(SOUTH, b.holes()+1, SOUTH)); //can't move with an invalid hole
    
    for(int i = 1; i <= b.holes(); i++)
    {
        b.moveToPot(SOUTH, i, SOUTH);
        b.moveToPot(opponent(SOUTH), i, opponent(SOUTH));
    }//move everything to pots
    assert(b.beansInPlay(SOUTH) == 0 && b.beansInPlay(NORTH) == 0); //test beansInPlay

    
}

void doPlayerTests()
{
    HumanPlayer hp("Marge");
    assert(hp.name() == "Marge"  &&  hp.isInteractive());
    BadPlayer bp("Homer");
    assert(bp.name() == "Homer"  &&  !bp.isInteractive());
    SmartPlayer sp("Lisa");
    assert(sp.name() == "Lisa"  &&  !sp.isInteractive());
    Board b(3,2);
    b.setBeans(SOUTH, 2, 0);
    cout << "=========" << endl;
    int n ;//= hp.chooseMove(b, SOUTH);
    cout << "=========" << endl;
    //assert(n == 1  ||  n == 3);
    //n = bp.chooseMove(b, SOUTH);
    //assert(n == 1  ||  n == 3);
    n = sp.chooseMove(b, SOUTH);
    assert(n == 1  ||  n == 3);
    
    b.setBeans(NORTH, 0, 2);
    b.setBeans(NORTH, 1, 3);
    b.setBeans(NORTH, 2, 3);
    b.setBeans(NORTH, 3, 0);
    b.setBeans(SOUTH, 0, 1);
    b.setBeans(SOUTH, 1, 0);
    b.setBeans(SOUTH, 2, 3);
    b.setBeans(SOUTH, 3, 0);
    
    n = sp.chooseMove(b, SOUTH);
    assert(n == 2); //can only move legally
    n = bp.chooseMove(b, SOUTH);
    assert(n == 2); //can only move legally
    
    b.setBeans(NORTH, 1, 0);
    b.setBeans(NORTH, 2, 0);
    b.setBeans(NORTH, 3, 0);
    b.setBeans(SOUTH, 1, 0);
    b.setBeans(SOUTH, 2, 0);
    b.setBeans(SOUTH, 3, 0);
    b.setBeans(NORTH, 0, 2);
    b.setBeans(SOUTH, 0, 2);
    
    n = sp.chooseMove(b, SOUTH);
    assert(n == -1); //while can't move, return -1
    n = bp.chooseMove(b, SOUTH);
    assert(n == -1); //while can't move, return -1

}

void doGameTests()
{
    SmartPlayer bp1("Bart");
    BadPlayer bp2("Homer");
    Board b(3,0);
    b.setBeans(SOUTH, 1, 2);
    b.setBeans(NORTH, 2, 1);
    b.setBeans(NORTH, 3, 2);
    Game g(b, &bp1, &bp2);
    bool over;
    bool hasWinner;
    Side winner;
    //    Homer
    //   0  1  2
    // 0         0
    //   2  0  0
    //    Bart
    g.display();
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, 0) == 0 && g.beans(SOUTH, 0) == 0 &&
           g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 1 && g.beans(NORTH, 3) == 2 &&
           g.beans(SOUTH, 1) == 2 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);
    
    g.move();
    //   0  1  0
    // 0         3
    //   0  1  0
    g.display();
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, 0) == 0 && g.beans(SOUTH, 0) == 3 &&
           g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 1 && g.beans(NORTH, 3) == 0 &&
           g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);
    
    g.move();
    //   1  0  0
    // 0         3
    //   0  1  0
    g.display();
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, 0) == 0 && g.beans(SOUTH, 0) == 3 &&
           g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
           g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);
    
    g.move();
    //   1  0  0
    // 0         3
    //   0  0  1
    g.display();
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, 0) == 0 && g.beans(SOUTH, 0) == 3 &&
           g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
           g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 1);
    
    g.move();
    //   0  0  0
    // 1         4
    //   0  0  0
    g.display();
    g.status(over, hasWinner, winner);
    assert(over && g.beans(NORTH, 0) == 1 && g.beans(SOUTH, 0) == 4 &&
           g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
           g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);
    assert(hasWinner && winner == SOUTH);
    //////////////////////////////////
    b.setBeans(NORTH, 0, 2);
    b.setBeans(NORTH, 1, 3);
    b.setBeans(NORTH, 2, 3);
    b.setBeans(NORTH, 3, 0);
    b.setBeans(SOUTH, 0, 1);
    b.setBeans(SOUTH, 1, 0);
    b.setBeans(SOUTH, 2, 3);
    b.setBeans(SOUTH, 3, 0);
    HumanPlayer bp3("Human");
    Game g1(b, &bp1, &bp3);
    //    Homer
    //   3  3  0
    // 2         1
    //   0  3  0
    //    Bart
    g1.display();
    
    g1.move();
    //   3  3  1
    // 2         2
    //   0  0  1
    g1.display();
    g1.status(over, hasWinner, winner);
    assert(!over && g1.beans(NORTH, 0) == 2 && g1.beans(SOUTH, 0) == 2 &&
           g1.beans(NORTH, 1) == 3 && g1.beans(NORTH, 2) == 3 && g1.beans(NORTH, 3) == 1 &&
           g1.beans(SOUTH, 1) == 0 && g1.beans(SOUTH, 2) == 0 && g1.beans(SOUTH, 3) == 1);
    
    g1.move();
    //   3  4  0
    // 9         2
    //   0  0  1
    g1.display();
    
    g1.move();
    //   0  0  0
    // 9         3
    //   0  0  0
    g1.display();
    g1.status(over, hasWinner, winner);
    assert(over && g1.beans(NORTH, 0) == 9 && g1.beans(SOUTH, 0) == 3 &&
           g1.beans(NORTH, 1) == 0 && g1.beans(NORTH, 2) == 0 && g1.beans(NORTH, 3) == 0 &&
           g1.beans(SOUTH, 1) == 0 && g1.beans(SOUTH, 2) == 0 && g1.beans(SOUTH, 3) == 0);
    assert(hasWinner && winner == NORTH);

    assert(!g1.move()); //can't move when the g1ame is over
    
    
    b.setBeans(NORTH, 1, 0);
    b.setBeans(NORTH, 2, 0);
    b.setBeans(NORTH, 3, 0);
    b.setBeans(SOUTH, 1, 0);
    b.setBeans(SOUTH, 2, 0);
    b.setBeans(SOUTH, 3, 0);
    b.setBeans(NORTH, 0, 2);
    b.setBeans(SOUTH, 0, 2);
    Game g2(b, &bp1, &bp3);
    
    g2.status(over, hasWinner, winner);
    assert(over && g2.beans(NORTH, 0) == 2 && g2.beans(SOUTH, 0) == 2 &&
           g2.beans(NORTH, 1) == 0 && g2.beans(NORTH, 2) == 0 && g2.beans(NORTH, 3) == 0 &&
           g2.beans(SOUTH, 1) == 0 && g2.beans(SOUTH, 2) == 0 && g2.beans(SOUTH, 3) == 0);
    assert(!hasWinner);
}



int main()
{
    doGameTests();
    doBoardTests();
    doPlayerTests();
    cout << "Passed all tests" << endl;
    SmartPlayer bp1("Bart");
    SmartPlayer bp2("Homer");
    Board b(3,2);

    Game g(b, &bp1, &bp2);
    g.play();
    
}

