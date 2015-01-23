//
//  Game.cpp
//  Project 3
//
//  Created by Li-Wei Tseng on 5/11/14.
//  Copyright (c) 2014 Li-Wei Tseng. All rights reserved.
//

#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "Side.h"
#include <iostream>
using namespace std;

Game::Game(const Board& b, Player* south, Player* north)
:m_b(b)
{
    m_south = south;
    m_north = north;
    m_turn = SOUTH; //south always move first
}
//Construct a Game to be played with the indicated players on a copy of the board b. The player on the south side always moves first.

void Game::display() const
{
    //display NORTH's name
    for(int i = 0; i < m_b.holes() - 1; i++)
    {
        cout << "  " ;
    }
    cout << m_north->name()<< endl;
    
    //display NORTH side
    for(int i = 1; i <= m_b.holes(); i++)
    {
        cout << "  " << m_b.beans(NORTH, i);
    }
    
    //display NORTH pots
    cout << endl << m_b.beans(NORTH, 0);
    
    for(int i = 1; i <= m_b.holes(); i++)
    {
        cout << "   ";
    }
    //display SOUTH pots
    cout << " " << m_b.beans(SOUTH, 0) << endl;
    
    //display SOUTH side
    for(int i = 1; i <= m_b.holes(); i++)
    {
        cout << "  " << m_b.beans(SOUTH, i);
    }
    
    //display SOUTH's name
    
    cout << endl;
    for(int i = 0; i < m_b.holes() - 1; i++)
    {
        cout << "  " ;
    }
    cout << m_south->name()<< endl;

}
//Display the game's board in a manner of your choosing, provided you show the names of the players and a reasonable representation of the state of the board.

void Game::status(bool& over, bool& hasWinner, Side& winner) const
{
    if(m_b.beansInPlay(SOUTH) != 0 && m_b.beansInPlay(NORTH) != 0)
    {
        over = false; //if more moves are possible, set over to false
        return;
    }
    over = true;
    if(m_b.beans(NORTH, 0) == m_b.beans(SOUTH, 0)) //if both sides' pots have same amount of beans, they tie and there's no winner
        hasWinner = false;
    else
    {
        //find which side's has more beans in its pot
        winner = (m_b.beans(NORTH, 0) > m_b.beans(SOUTH, 0)) ? NORTH : SOUTH;
        hasWinner = true;
    }
}
//If the game isn't over (i.e., more moves are possible), set over to false and do not change anything else. Otherwise, set over to true and hasWinner to true if the game has a winner, or false if it resulted in a tie. If hasWinner is set to false, leave winner unchanged; otherwise, set it to the winning side.

bool Game::move()
{
    bool over, hasWinner;
    Side winner;
    
    status(over, hasWinner, winner);
    if(over)
    {
        return false; //return false if the game is over
    }
    
    ///////////////////////when the game isn't over/////////////
    
    Side endSide;
    int endHole;
    Player* tempPlayer;
    int hole;
    //Board tempBoard(m_b); //tempBoard holds the original beans before sow
    
    
    tempPlayer = (m_turn == NORTH) ? m_north : m_south;
    
    hole = tempPlayer->chooseMove(m_b, m_turn);
    cout << tempPlayer->name() << " chooses hole " << hole << endl;
    
    m_b.sow(m_turn, hole, endSide, endHole);
    
    if(endHole != 0 && endSide == m_turn && m_b.beans(m_turn, endHole) - 1 == 0 && m_b.beans(opponent(m_turn), endHole) != 0)
    {
        //If it was placed in one of the player's own holes that was empty just a moment before, and if the opponent's hole directly opposite from that hole is not empty, capture the beans.
        m_b.moveToPot(m_turn, endHole, m_turn);
        m_b.moveToPot(opponent(m_turn), endHole, m_turn);
    }
    
    while(endHole == 0 && m_turn == endSide)
    {
        //if the bean ends at the the player's pot, it gets another turn, display the result.
        display();
        cout << tempPlayer->name() << " gets another turn." << endl;
        hole = tempPlayer->chooseMove(m_b, m_turn);
        if(hole == -1)
            break;
        
        if(!m_b.sow(m_turn, hole, endSide, endHole))
            break;

        
        cout << tempPlayer->name() << " chooses hole " << hole << endl;
    }
    
    
    if(m_b.beansInPlay(SOUTH) == 0)
    {
        display();

        cout << "Sweeping remaining beans into "<< m_north->name() <<"'s pot."<< endl;
        for(int i = 1; i <= m_b.holes(); i++)
        {
            m_b.moveToPot(m_turn, i, m_turn);
            m_b.moveToPot(opponent(m_turn), i, opponent(m_turn));
        }//move the remaining beans to the pots when the game is over
        display();
    }
    else if(m_b.beansInPlay(NORTH) == 0)
    {
        display();

        cout << "Sweeping remaining beans into "<< m_south->name() <<"'s pot."<< endl;
        for(int i = 1; i <= m_b.holes(); i++)
        {
            m_b.moveToPot(m_turn, i, m_turn);
            m_b.moveToPot(opponent(m_turn), i, opponent(m_turn));
        }//move the remaining beans to the pots when the game is over
        display();
    }
    
    m_turn = opponent(m_turn);//set the turn to the other player's
    
    return true;
}
//If the game is over, return false. Otherwise, make a complete move for the player whose turn it is (so that it becomes the other player's turn) and return true. "Complete" means that the player sows the seeds from a hole and takes any additional turns required or completes a capture. If the player gets an additional turn, you should display the board so someone looking at the screen can follow what's happening.

void Game::play()
{
    bool over, hasWinner;
    Side winner;
    Player* tempPlayer;
    
    do
    {
        display();
        
        if(!m_south->isInteractive() && !m_north->isInteractive())
        {
            cout << "Press ENTER to continue ";
            cin.ignore();
        }
        move();
        status(over, hasWinner, winner);
    }while(!over);
    
    if(hasWinner)
    {
        tempPlayer = (winner == NORTH) ? m_north : m_south;
        cout << "The winner is " << tempPlayer->name() << endl;
    }
    else
        cout << "The game restulted in a tie." << endl;
}
//Play the game. Display the progress of the game in a manner of your choosing, provided that someone looking at the screen can follow what's happening. If neither player is interactive, then to keep the display from quickly scrolling through the whole game, it would be reasonable periodically to prompt the viewer to press ENTER to continue and not proceed until ENTER is pressed. (The ignore function for input streams is useful here.) Announce the winner at the end of the game. You can apportion to your liking the responsibility for displaying the board between this function and the move function.

int Game::beans(Side s, int hole) const
{
    if(hole < 0 || hole > m_b.holes())
        return -1; //return -1 if the hole humber is invalid.
    
    return m_b.beans(s, hole);
}
//Return the number of beans in the indicated hole or pot of the game's board, or −1 if the hole number is invalid. This function exists so that we and you can more easily test your program.
