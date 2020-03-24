//
//  game.hpp
//  chessMaster
//
//  Created by Karan Mukhi on 08/04/2019.
//  Copyright © 2019 Karan Mukhi. All rights reserved.
//
//  --------------------------------------------------------------------------------------------
//  ###################################  GAME CLASS DEFINITION  ###############################
//  --------------------------------------------------------------------------------------------
//
//  This class is an overarching class that contains the board and player objects.
//  It is used for interfacing between the player and board objects, taking user inputs
//  from players and passing them to the board
//  All exceptions to user inputs are caught within this class
//



#ifndef game_hpp
#define game_hpp

#include <stdio.h>
#include <iostream>
#include <ctime>
#include <memory>
#include "player.hpp"
#include "board.hpp"

class game {
private:
    std::unique_ptr<player> white;
    std::unique_ptr<player> black;
    std::shared_ptr<Board::board> boardPoint;
    
public:
    game(); //Constructor
    ~game(){std::cout << "Ending game\n";} //Destructor
    void play(); //Function that deals with all game play
};

#endif /* game_hpp */
