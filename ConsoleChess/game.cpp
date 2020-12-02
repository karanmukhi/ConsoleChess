//
//  game.cpp
//  chessMaster
//
//  Created by Karan Mukhi on 08/04/2019.
//  Copyright © 2019 Karan Mukhi. All rights reserved.
//

#include "game.hpp"



//  ############################################################################################
//  --------------------------------------------------------------------------------------------
//  ###################################  GAME MEMBER FUNCTION  #################################
//  --------------------------------------------------------------------------------------------
//  ############################################################################################


//Game constructor
game::game(){
    //Construcst a game object with two players and a board properly set up
    boardPoint = std::make_shared<Board::board>(); //initialise the board
    boardPoint->fillBoard(); //Fill the board with squares and pieceses
    std::string player1, player2; //Player names

    std::cout << "\n\n\n\nInitialising new game.\nPlease enter the name of player one:\n";
    std::getline(std::cin, player1);
    std::cout<< "Please enter the name of player two:\n";
    std::getline(std::cin, player2);
    //Randomly assign players to colours
    srand(static_cast<unsigned int>(time(NULL)));
    int randomval = rand() % 2;
    if(randomval){
        //Initialise palyers
        white = std::make_unique<player>(player1,true);
        black = std::make_unique<player>(player2,false);
    }
    else {
        //Initialise players
        white = std::make_unique<player>(player2,true);
        black = std::make_unique<player>(player1,false);
    }
    std::cout<< white->getName() << " is white. " << black->getName() << " is black.\n" << white->getName() << " to move first.\n";
}



void game::play(){
    //Plays the game, while the board is not in a state of checkmate it continues
    bool checkmate(false);
    bool turn(true);
    Code::moveCode move;
    
    while(!checkmate){
        boardPoint->print(); //Prints board
        while(true){
            try{
                if(turn){ //If white's turn
                    std::cout << white->getName();
                    move = white->getMove(); //Get move - this can throw a number of exceptions
                }
                else{ //If black's turn
                    std::cout << black->getName();
                    move = black->getMove();
                }
                boardPoint->move(turn, move); //Make the move - this can throw a number of excpetions
                break;
            }
            catch(int errorFlag)
            {   //Catch exceptions
                //Bad format
                if(errorFlag == 1) {std::cout << "Invalid input move. Please enter in the form 'Initial Colum''Initial Row' to 'Final Column''Final Row'. Eg. E2 to E4\n";}
                if(errorFlag == 2) {std::cout << "Invalid input move. Please enter in the form 'Initial Colum''Initial Row' to 'Final Column''Final Row'. Eg. E2 to E4\n";}
                if(errorFlag == 3) {std::cout << "Invalid input move. Please enter in the form 'Initial Colum''Initial Row' to 'Final Column''Final Row'. Eg. E2 to E4\n";}
                //Illegal moves
                if(errorFlag == 4) {std::cout << "Invalid move: No piece on this square\n";}
                if(errorFlag == 5) {std::cout << "Invalid move: You cannot take same coloured piece\n";}
                if(errorFlag == 6) {std::cout << "Invalid move: Illegal move\n";}
                if(errorFlag == 8) {std::cout << "Invalid move: There is a piece in the way.\n";}
                if(errorFlag == 9) {std::cout << "Invalid move: You cannot move opposition pieces\n";}
                if(errorFlag == 10) {std::cout << "Invalid move: You are in check you cannot make this move\n";}
                if(errorFlag == 10) {std::cout << "Invalid move: You castle while in or through check\n";}
                //Pawn promotion
                if(errorFlag == 7) {
                    std::cout << "Pawn promoted! Please enter what you would like to promote it to:\n'Q' - Queen\n'R' - Rook\n'B' - Bishop\n'K' - Knight\n";
                    std::string input;
                    char pieceType, pieceColour;
                    if(turn) pieceColour = 'W';
                    else pieceColour = 'B';
                    while(true){
                        std::getline(std::cin, input);
                        if(input == "Q") {pieceType = 'Q'; break;}
                        else if(input == "R") {pieceType = 'R'; break;}
                        else if(input == "B") {pieceType = 'B'; break;}
                        else if(input == "K") {pieceType = 'N'; break;}
                        else std::cout << "Invalid Input. Please enter what you would like to promote it to:\n'Q' - Queen\n'R' - Rook\n'B' - Bishop\n'N' - Knight\n";
                    }
                    boardPoint->promote(pieceType, pieceColour, move); //Promotes pawn
                    break;
                }
            }
        }
        
        turn = !turn;
        checkmate = boardPoint->checkmate(turn); //Checks if board is in a state of checkmate
    }
    this->boardPoint->print();
    std::cout << "CHECKMATE!\n";
    //Prints winner
    if(!turn){
        std::cout << white->getName() << " wins!\n";
    }
    else{
        std::cout << black->getName() << " wins!\n";
    }
}



