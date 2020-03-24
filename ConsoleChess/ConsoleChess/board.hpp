//
//  board.hpp
//  chessMaster
//
//  Created by Karan Mukhi on 08/04/2019.
//  Copyright © 2019 Karan Mukhi. All rights reserved.
//
//  This header file contains the class definitions for the board and square classes
//  These are the objects comprise the platform that the game is played on
//
//  ---------------------------------------------------------------------------------
//  ###################################  BOARD CLASS   ###############################
//  ---------------------------------------------------------------------------------
//  The board class contains all information about the game and methods of accessing
//  and manipulating this data.
//  It can make moves, by manipulating the positions of pieces on the board, including
//  more advanced moves such as castling
//  It can assess if a player is in check or checkmate, by accessing data on the board

//  ---------------------------------------------------------------------------------
//  ###################################  SQUARE CLASS   #############################
//  ---------------------------------------------------------------------------------
//  This class stores the data for each location on the board
//  It can access data from the board and the pieces that occupy the square itself
//  It can check if moves are legal and compelete these moves
//  Essentially it is a way of interfacing between the board and the pieces


#ifndef board_hpp
#define board_hpp

#include <stdio.h>
#include <vector>
#include <memory>
#include <string>
#include "piece.hpp"


class game; //Forward declaration of game class
class square; //Forward declaration of square class


namespace Board{
class board {
private:
    std::shared_ptr<std::vector<std::vector<square> > > squarePoint;

public:
    //Constructor functions
    board(); //Initialises boardPoint
    void fillBoard(); //Fills boardPoint with 8 rows of 8 squares and all pieces set up correctly
    std::vector<square> backRow(bool colour); //Forms row of non-pawn pieces
    std::vector<square> frontRow(bool colour); //Forms row of pawns
    std::vector<square> emptyRow(bool colour); //Forms empty row
    
   //Display functions
    void print() const; //Prints the whole board
    void printRow(const std::vector<square> & printSquare, int rowNumb) const; //Prints a row in the board
    
    //Movement
    bool clearPath(const Code::moveCode & move) const; //Returns true if there is a clear path between initial and final squares
    void move(bool colour, Code::moveCode & move); //Player of colour, colour makes movement
    void castle(Code::moveCode & move); //Implements the castle-in rule
    void promote(char pieceType, char pieceColour, Code::moveCode move); //Implements the promotion rule
    
    //Check and checkmate functions
    void resetEnPassant(bool colour);
    Code::squareCode findPiece(bool colour, char type) const; //Returns the position of piece of colour and type
    bool check(bool colour) const; // Checks if player of colour is in check
    bool underAttack(bool colour, Code::squareCode attackPositioin) const; // Checks square attackPosition is under attack
    bool checkmate(bool colour) const; // Checks if player of colour is in checkmate
    
    // Access functions
    square * operator()(int col, int row); //Returns pointer to square col,row
    square * operator()(int col, int row) const; //Const of above
    square * operator()(Code::squareCode targetSquare); //Returns pointer of squareCode
    square * operator()(Code::squareCode targetSquare) const; //Const above
    bool validSquare(Code::squareCode valid) const; //Checks if square 'valid' is on the board ie 1<=col&row<=8
    
};
}

class square{
private:
    bool squareColour; //Colour of the square
    std::unique_ptr<piece> occupation; //Pointer to piece occupying square, default null
    Board::board* homeBoard; //Pointer to board
public:
    square(const std::string pieceName, const bool inputColour, Board::board* homeBoardInput); //Constructor
    bool getOccupation() const {if(occupation) return true; else return false;} //Check if a piece is occupying the square
    Board::board* getHomeBoard() const {return homeBoard;} //Returns a pointer to the board the square is contained in
    std::unique_ptr<piece> & getPiecePoint() {return occupation;} //Get a pointer to the piece that is occupying the square
    void setPiece(std::unique_ptr<piece> & newPiece){occupation = std::move(newPiece);} //Moves a piece to the square
    void removePiece(){occupation.reset();} //Removes the piece on the square
    void newPiece(char pieceType, char colour, char pieceSide); //Sets a new piece on the square
    bool getColour() const {return squareColour;} //Gets the colour of the square
    void printLine(const int line) const; //Prints a line in of the square;
    char getPieceColour() const{if(!getOccupation()) return '-'; else return occupation->getColour();} //Gets the colour of the pice
    char getPieceType() const {if(!getOccupation()) return '-'; else return occupation->getType();} //Gets the type code of the piece
    bool getSquareColour(){return squareColour;} //Gets the square colour
    void move(bool colour, Code::moveCode & target); //Processes checks for move using this as the intial square of the move
    void completeMove(Code::moveCode & target); // Completes the move
    bool legalCapture(Code::moveCode & target) const {return occupation->legalMove(target, true);} //Checks if the piece occupying the square can capture pieice on target.finalSquare
    bool castle(bool colour, Code::moveCode & target); //Proceses the checks for castling
    bool checkBlockMove(bool colour, Code::moveCode & move); //Checks if a move blocks check
};


#endif /* board_hpp */
