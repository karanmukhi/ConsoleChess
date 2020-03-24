//
//  player.cpp
//  chessMaster
//
//  Created by Karan Mukhi on 08/04/2019.
//  Copyright © 2019 Karan Mukhi. All rights reserved.
//

#include "player.hpp"
Code::squareCode interpret(std::string stringSquare){
    //Interprets the user input and converts to squareCode structure - a number between 1 and 8 for the row and column. Throws error code 2 or 3 for bad column or row, respectively.
    char col(stringSquare[0]), rowChar(stringSquare[1]);
    int row = rowChar - '0';
    Code::squareCode result;
    switch(col){
        case 'A':
        case 'a':
            result.col = 1;
            break;
        case 'B':
        case 'b':
            result.col = 2;
            break;
        case 'C':
        case 'c':
            result.col = 3;
            break;
        case 'D':
        case 'd':
            result.col = 4;
            break;
        case 'E':
        case 'e':
            result.col = 5;
            break;
        case 'F':
        case 'f':
            result.col = 6;
            break;
        case 'G':
        case 'g':
            result.col = 7;
            break;
        case 'H':
        case 'h':
            result.col = 8;
            break;
        default: throw 2;
    }
    if(row < 0 || row > 8) throw 3;
    else result.row = row;
    return result;
}

player::player(std::string playerName, bool playerColour){
    //Player constructor, sets name and colour
    name = playerName;
    colour = playerColour;
}

Code::moveCode player::getMove() const{
    //Prompts players to enter a move and returns a moveCode struct. Errors 1,2,3 are thrown if the input is faulty.
    Code::moveCode move;
    std::string input, stringSquare;
    std::cout << " please enter a move:\n";
    std::getline(std::cin, input);
    size_t space = input.find(" to");
    if(space!= 2) throw 1;
    stringSquare = input.substr(0,2);
    move.initialSquare = interpret(stringSquare);
    stringSquare = input.substr(6,2);
    move.finalSquare = interpret(stringSquare);
    //std::cin >> move;
    return move;
}

/*
 //Tried overloadign the input stream for the moveCode structure, however there were bugs and limited time to fix
 
std::istream &operator>>(std::istream &is, Code::moveCode& move){
    std::string input, stringSquare;
    std::getline(std::cin, input);
    size_t space = input.find(" to");
    if(space!= 2) throw 1;
    stringSquare = input.substr(0,2);
    move.initialSquare = interpret(stringSquare);
    stringSquare = input.substr(6,2);
    move.finalSquare = interpret(stringSquare);
    return is;
}
*/
