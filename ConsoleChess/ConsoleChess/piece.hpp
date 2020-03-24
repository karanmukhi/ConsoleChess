//
//  piece.hpp
//  chessMaster
//
//  Created by Karan Mukhi on 08/04/2019.
//  Copyright © 2019 Karan Mukhi. All rights reserved.
//  ----------------------------------------------------------------------------------
//  ###################################  PIECE CLASS   #############################
//  ---------------------------------------------------------------------------------
//  Piece is an abstract base class, from which all other pieces in the game of chess
//  can be derived
//  The pieces contain data about their colour and homesquare
//  It has a method of accessing data via its homesquare and hence the entire board
//  Individual pieces are derived from this abstrace base class
//  Member functions are overridden for each unique piece to define the contrainst it
//  abides by when moving
// 


#ifndef piece_hpp
#define piece_hpp
#include "player.hpp"
#include <stdio.h>


class square;

class piece {
    //Abstract base class for all pieces
protected:
    char colour;
    char side; //Which side of the board the piece is - 'K' or 'Q' for back pieces, A->H for pawns
    char type; //Name code of piece
    square* homeSquare; //Pointer to board
public:
    virtual ~piece(){} //Destructor
    virtual bool legalMove(const Code::moveCode & move, bool noThrow) const = 0; //Pure virtual function, for legal checking if a move is legal
    void setHomeSquare(square* homeSquareInput){homeSquare = homeSquareInput;}
    square* getHomeSquare() const {return homeSquare;}
    char getColour() const {return colour;}
    char getSide() const {return side;}
    char getType() const {return type;}
    virtual void switchMoved(){}//Virutal function overridden by castlePiece and pawn, set if a piece has moved
    virtual void resetMoved(){}//Virutal function overridden by castlePiece and pawn, used to reset if a piece has moved
    bool virtual getMoved() const {return false;}//Virutal function overridden by castlePiece and pawn, used for tracking if a piece has moved
};

class linePiece: public piece {
    //A class for all pieces that travel back and forwards in straight lines, i.e. king, queen, rook and bishop
protected:
    bool straight; //Sets whether the piece can move in straight lines
    bool diagonal; //Sets whether the piece can move on diagonals
    bool distance; //Sets how far the piece can move, false for the king
public:
    bool legalMove(const Code::moveCode & move, bool noThrow) const;
   
};

class castlingPiece: public linePiece{
    //A class for pieces involved in castling - king and rook
protected:
    bool hasMoved; //Tracks if the piece has ever moved
public:
    void switchMoved(){hasMoved = true;} //Swithes if the piece moves
    bool getMoved() const {return hasMoved;} //Tracks if the piece has moved
};

class king: public castlingPiece {
public:
    king(char inputColour){
        colour = inputColour;
        side = 'K';
        type = 'K';
        straight = true;
        diagonal = true;
        distance = false;
        hasMoved = false;
    }
};

class queen: public linePiece {
public:
    queen(char inputColour, square* homeSquareInput){
        homeSquare = homeSquareInput;
        colour = inputColour;
        side = 'Q';
        type = 'Q';
        straight = true;
        diagonal = true;
        distance = true;
    }
    ~queen(){std::cout<<"Queen taken\n";}
    
};

class rook: public castlingPiece {

public:
    rook(char inputColour, char inputSide, square* homeSquareInput){
        homeSquare = homeSquareInput;
        colour = inputColour;
        side = inputSide;
        type = 'R';
        straight = true;
        diagonal = false;
        distance = true;
        hasMoved = false;
        
    }
     ~rook(){std::cout<<"Rook taken\n";}
};

class bishop: public linePiece {
public:
    bishop(char inputColour, char inputSide, square* homeSquareInput){
        homeSquare = homeSquareInput;
        colour = inputColour;
        side = inputSide;
        type = 'B';
        straight = false;
        diagonal = true;
        distance = true;
    }
    ~bishop(){std::cout<<"Bishop taken\n";}
    
};

class knight: public piece {
public:
    knight(char inputColour, char inputSide, square* homeSquareInput){
        homeSquare = homeSquareInput;
        colour = inputColour;
        side = inputSide;
        type = 'N';
    }
    ~knight(){std::cout<<"Knight taken\n";}
    bool legalMove(const Code::moveCode & move, bool noThrow) const;
};

class pawn: public piece{
private:
    bool justMoved;
public:
    pawn(char inputColour, char inputSide, square* homeSquareInput){
        homeSquare = homeSquareInput;
        colour = inputColour;
        side = inputSide;
        type = 'P';
        justMoved = false;
    }
    ~pawn(){std::cout<<"Pawn taken\n";}
    void switchMoved(){justMoved = true;} //Swithces if the piece has just moved
    void resetMoved(){justMoved = false;}//Resets when the piece has not moved in the last turn
    bool getMoved() const {return justMoved;} //Tracks if the particle has jsut moved
    bool legalMove(const Code::moveCode & move, bool noThrow) const;
    bool legalCapture(const Code::moveCode & move, bool noThrow) const;
};
#endif /* piece_hpp */
