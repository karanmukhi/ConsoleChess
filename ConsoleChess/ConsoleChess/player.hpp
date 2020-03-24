//
//  player.hpp
//  chessMaster
//
//  Created by Karan Mukhi on 08/04/2019.
//  Copyright © 2019 Karan Mukhi. All rights reserved.
//  --------------------------------------------------------------------------------------------
//  ###################################  PLAYER CLASS DEFINITION  #############################
//  --------------------------------------------------------------------------------------------
//
//  This class is used to interface with the users
//  taking their names and creating player objects for each of the two players
//  It prompts users to enter moves which are then taken by the game object
//
//  This header file also contains definitions for two structures, used in interpreting
//  locations on the board and moves. squareCode specifies a location on the board
//  moveCode specifies a move from an initial position on the board to a final position


#ifndef player_hpp
#define player_hpp

#include <stdio.h>
#include <string>
#include <iostream>


namespace Code{
    struct squareCode{
    int row;
    int col;
    squareCode(){row = 0; col = 0;}
    void print() const { std::cout  << col << "," << row << "    ";}
};


struct moveCode{
    squareCode initialSquare;
    squareCode finalSquare;
};
}

class player {
    
private:
    bool colour;
    std::string name;
public:
    player(std::string playerName, bool playerColour);
    ~player(){std::cout << name << " Player's Object deleted\n";}
    void setColour(bool colour);
    bool getColour() const {return colour;}
    std::string getName() const {return name;}
    Code::moveCode getMove() const;
};
#endif /* player_hpp */
