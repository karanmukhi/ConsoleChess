//
//  board.cpp
//  chessMaster
//
//  Created by Karan Mukhi on 08/04/2019.
//  Copyright © 2019 Karan Mukhi. All rights reserved.
//

#include "board.hpp"


using namespace Board;


//  ############################################################################################
//  --------------------------------------------------------------------------------------------
//  ###################################  BOARD MEMBER FUNCTION  ###############################
//  --------------------------------------------------------------------------------------------
//  ############################################################################################

board::board(){
    //Constructor, this just makes a vector of vectors of squares.
    //Needs to be done prior to constructing the squares as the board pointer
    //need to be initilaised before sharing it with the squares
    squarePoint = std::make_shared<std::vector<std::vector<square>>>();
}

void board::fillBoard(){
    //Called straight after the board is initialised
    //Fills the squarePoint vector<vector<square>> with squares and sets pieces on
    //these squares in teh correct configuration
    squarePoint->push_back(backRow(false)); //Sets a row of squares with white non-pawn piece
    squarePoint->push_back(frontRow(true)); //Sets a row of squares with white pawns piece
    bool startColour(false);
    for(int i(0); i<4; i++){
        //Sets 4 empty rows, with alternating starting colour
        squarePoint->push_back(emptyRow(startColour));
        startColour = !startColour;
    }
    squarePoint->push_back(frontRow(false));//Sets a row of squares with black pawns piece
    squarePoint->push_back(backRow(true)); //Sets a row of squares with black non-pawn piece
    for(int rowNumb(0); rowNumb<2; rowNumb++){ //Input homeSquare to pieces
        for(int i(0); i<8; i++){
            //std::cout<<rowNumb<<" "<< i << " "<<(*this)(i+1,rowNumb+1)<<std::endl;
            (*this)(i+1,rowNumb+1)->getPiecePoint()->setHomeSquare((*this)(i+1,rowNumb+1));
            (*this)(i+1,8-rowNumb)->getPiecePoint()->setHomeSquare((*this)(i+1,8-rowNumb));
        }
    }
}

std::vector<square> board::backRow(bool colour) {
    //Forms a row of squares with non-pawn pieces
    //The colour of the pieces as well as the starting square colour  is specifed by the input "colour"
    char pieceColour;
    bool squareColour(colour);
    int rowNumb;
    if(colour) rowNumb = 8;
    else rowNumb = 1;
    std::string pieces[8]{"QR","QN","QB","QQ","KK","KB", "KN", "KR"};
    if(colour) pieceColour = 'B';
    else pieceColour = 'W';
    std::vector<square> result;
    for(int i(0); i<8; i++){
        result.push_back(square(pieceColour+pieces[i], squareColour, this));
        squareColour = !squareColour;
    }
    return result;
}


std::vector<square> board::frontRow(bool colour){
    //Forms a row of squares with pawn pieces
    //The colour of the pieces as well as the starting square colour  is specifed by the input "colour"
    char pieceColour;
    bool squareColour(colour);
    if(colour) pieceColour = 'W';
    else pieceColour = 'B';
    int rowNumb;
    if(colour) rowNumb = 2;
    else rowNumb = 7;
    std::string pieces[8]{"1P","2P","3P","4P","5P","6P", "7P", "8P"};
    std::vector<square> result;
    for(int i(0); i<8; i++){
        result.push_back(square(pieceColour+pieces[i], squareColour, this));
        squareColour = !squareColour;
    }
    return result;
}


std::vector<square> board::emptyRow(bool colour){
    //Forms an empty row of squares
    //The colour of the first square is specified by the input colour
    std::vector<square> result;
    for(int i(0); i<8; i++){
        result.push_back(square("-", colour, this));
        colour = !colour;
    }
    return result;
}





void board::print() const{
    //Prints the board in a visually appealing way for the user, with a boarder
    char colLetter[8] = {'A', 'B', 'C' , 'D', 'E', 'F', 'G', 'H'}; //Labels for the columns
    std::cout << "\n\n\n\n         ";
    std::cout <<"|";
    for(int i(0); i<8; i++){
        std::cout << "-------" << colLetter[i] << "------";
    }
    std::cout <<"|";
    std::cout << std::endl;
    int rowNumb(0);
    for_each(squarePoint->rbegin(), squarePoint->rend(), [&rowNumb, this] (const std::vector<square> & row){
        //Lambda function to print each row in the board
        rowNumb++;
        printRow(row, rowNumb);
    });
    std::cout << "         ";
    std::cout <<"|";
    for(int i(0); i<8; i++){
        std::cout << "-------" << colLetter[i] << "------";
    }
    std::cout <<"|";
    std::cout << "\n\n\n\n";
    
}

void board::printRow(const std::vector<square> & row, int rowNumb) const {
    //Prints a row in the board in a visually appealing way
    for(int i(0); i<7; i++){
        std::cout << "         ";
        if(i==3) std::cout << 9-rowNumb;
        else std::cout << "|";
        for(int j(0); j<8; j++){
            
            row[j].printLine(i);
            
        }
        if(i==3) std::cout << 9-rowNumb;
        else  std::cout << "|";
        std::cout << std::endl;
    }
}

//  ######## Access functions  ########
// Returns a pointer to a square in the board specified by a squareCode or two integers
square * board::operator()(int col, int row){
    return &(*(squarePoint))[row-1][col-1];
}

square * board::operator()(int col, int row) const{
    return &(*(squarePoint))[row-1][col-1];
}

square * board::operator()(Code::squareCode targetSquare){
    return &(*(squarePoint))[targetSquare.row - 1][targetSquare.col - 1];
}

square * board::operator()(Code::squareCode targetSquare) const{
    return &(*(squarePoint))[targetSquare.row - 1][targetSquare.col - 1];
}


void Board::board::move(bool colour, Code::moveCode & move){
    //Takes a moveCode input and passes this to the initial square to check if the move is legal
    //before completing the move
    
    this->resetEnPassant(colour); //Resets the enpassant marker on each pawn of colour colour, so they are no open to being captured by enpassant
    (*this)(move.initialSquare)->move(colour, move); //Passes the move code on the the initial square
    if(this->check(!colour)) std::cout << "CHECK!\n"; //If the board's state after the move is check for the other player "CHECK!"  is output on the terminal
}

void Board::board::promote(char pieceType, char pieceColour, Code::moveCode move){
    //For pawn promotion, if the pawn at the end is in the
    (*this)(move.initialSquare)->removePiece();
    (*this)(move.finalSquare)->newPiece(pieceType, pieceColour, 'K');
}


//  -----------------------
//  SQUARE MEMBER FUNCTION
//  -----------------------


bool Board::board::clearPath(const Code::moveCode & move) const{
    //Checks if there are any pieces in the path of a move of a straight piece. Returns true if there are none.
    Code::squareCode toCheck;
    if(move.initialSquare.row == move.finalSquare.row){ //Up && Down
        toCheck.row = move.initialSquare.row;
        if(move.initialSquare.col<move.finalSquare.col){ //Up
            for(int i(move.initialSquare.col+1); i<move.finalSquare.col; i++){
                toCheck.col = i;
                if((*this)(toCheck)->getOccupation())return false;
            }
        }
        else if(move.initialSquare.col > move.finalSquare.col){ //Down
            for(int i(move.finalSquare.col+1); i<move.initialSquare.col; i++){
                toCheck.col = i;
                if((*this)(toCheck)->getOccupation()) return false;
            }
        }
        return true;
    }
    
    if(move.initialSquare.col == move.finalSquare.col){ //Left and Right
        toCheck.col = move.initialSquare.col;
        if(move.initialSquare.row<move.finalSquare.row){ //Right
            for(int i(move.initialSquare.row+1); i<move.finalSquare.row; i++){
                toCheck.row = i;
                if((*this)(toCheck)->getOccupation()) return false;
            }
        }
        else if(move.initialSquare.row > move.finalSquare.row){ //Left
            for(int i(move.finalSquare.row+1); i<move.initialSquare.row; i++){
                toCheck.row = i;
                if((*this)(toCheck)->getOccupation()) return false;
            }
        }
        return true;
    }
    

    if(move.finalSquare.row - move.initialSquare.row == move.finalSquare.col - move.initialSquare.col){ //South West -> North East line
        if(move.initialSquare.col < move.finalSquare.col){ //North East
            toCheck = move.initialSquare;
            for(int i(move.initialSquare.col+1); i<move.finalSquare.col; i++){
                toCheck.col++;
                toCheck.row++;
                if((*this)(toCheck)->getOccupation()) return false;
            }
        }
        else if(move.initialSquare.col > move.finalSquare.col){ //South East
            toCheck = move.finalSquare;
            for(int i(move.finalSquare.col+1); i<move.initialSquare.col; i++){
                toCheck.col++;
                toCheck.row++;
                if((*this)(toCheck)->getOccupation()) return false;
            }
        }
        return true;
    }
    
    if(move.finalSquare.row - move.initialSquare.row == move.initialSquare.col - move.finalSquare.col){ //South East -> North West Line
        if(move.initialSquare.col < move.finalSquare.col){ //South West
            toCheck = move.initialSquare;
            for(int i(move.initialSquare.col+1); i<move.finalSquare.col; i++){
                toCheck.col++;
                toCheck.row--;
                if((*this)(toCheck)->getOccupation()) return false;
            }
        }
        else if(move.initialSquare.col > move.finalSquare.col){ //North West
            toCheck = move.finalSquare;
            for(int i(move.finalSquare.col+1); i<move.initialSquare.col; i++){
                toCheck.col++;
                toCheck.row--;
                if((*this)(toCheck)->getOccupation()) return false;
            }
        }
        return true;
    }
    else return true;
}

void board::castle(Code::moveCode &move){
    //Performs the castling move
    if(move.finalSquare.col == 3){
        //Castling to the left
        (*this)(move.initialSquare)->completeMove(move);
        Code::moveCode rookMove;
        rookMove.finalSquare.row = move.initialSquare.row;
        rookMove.initialSquare.row = move.initialSquare.row;
        rookMove.finalSquare.col = 4;
        rookMove.initialSquare.col = 1;
        (*this)(rookMove.initialSquare)->completeMove(rookMove);
        return;
    }
    else if(move.finalSquare.col == 7){
        //Castling to the right
            (*this)(move.initialSquare)->completeMove(move);
            Code::moveCode rookMove;
            rookMove.finalSquare.row = move.initialSquare.row;
            rookMove.initialSquare.row = move.initialSquare.row;
            rookMove.finalSquare.col = 6;
            rookMove.initialSquare.col = 8;
            (*this)(rookMove.initialSquare)->completeMove(rookMove);
    }
}

void board::resetEnPassant(bool colour){
    //Reset the enPassant marker for all pawns of colour Colour
    int rowNumb;
    if(colour) rowNumb = 4;
    else rowNumb = 5;
    for(int i(1); i<=8; i++){
        if((*this)(i,rowNumb)->getOccupation()){
            if((*this)(i,rowNumb)->getPiecePoint()->getType() == 'P'){
                (*this)(i,rowNumb)->getPiecePoint()->resetMoved();
            }
        }
    }
}

Code::squareCode board::findPiece(bool colour, char type) const{
    //Finds Piece of colour and type, returns squareCode col = row = 0 if piece does not exist.
    Code::squareCode position;
    int rowNumb(0), colNumb(0);
    for_each(squarePoint->begin(), squarePoint->end(), [&](const std::vector<square> & row) ->Code::squareCode{
        rowNumb++;
        colNumb = 0;
        for_each(row.begin(), row.end(), [&](const square & thisSquare)->Code::squareCode{
            colNumb++;
            if(thisSquare.getPieceType() == type){
                if((thisSquare.getPieceColour() == 'W' && colour) || (thisSquare.getPieceColour() == 'B' && !colour)){
                    position.row = rowNumb;
                    position.col = colNumb;
                }
            }
            return position;
        });
        return position;
    });
    return position;
}

bool Board::board::check(bool colour)const{
    //Returns true if 'colour' is in Check
    //Iterate through all other pieces see if one could capture king if can return true
    bool result(false);
    Code::squareCode kingSquare;
    kingSquare = this->findPiece(colour, 'K');
    result = this->underAttack(colour, kingSquare);
    return result;
    
}

bool board::underAttack(bool colour, Code::squareCode attackPosition) const{
    //Checks if square attackPosition can be attacked by other colour
    bool result(false);
    Code::moveCode move;
    Code::squareCode thisPosition;
    int rowNumb(0), colNumb(0);
    move.finalSquare = attackPosition;
    for_each(squarePoint->begin(), squarePoint->end(), [&] (const std::vector<square> & row){
        rowNumb++;
        colNumb = 0;
        for_each(row.begin(), row.end(), [&] (const square & thisSquare){
            colNumb++;
            thisPosition.col = colNumb;
            thisPosition.row = rowNumb;
            //std::cout << "\nThis Positions: ";
            //thisPosition.print();
            //std::cout << "To attack: ";
            //attackPosition.print();
            move.initialSquare = thisPosition;
            if((thisSquare.getPieceColour() == 'W' && !colour) || (thisSquare.getPieceColour() == 'B' && colour)){
                //std::cout << "Piece found";
                if(thisSquare.legalCapture(move)) {result = true;} //std::cout<< " Capture";}
            }
            return result;
        });
        return result;
    });
    return result;
}

bool Board::board::validSquare(Code::squareCode valid) const {
    //Checks if the square 'valid' is on the board - 0 < x < 9
    if(valid.row < 1 || valid.row > 8) return false;
    if(valid.col < 1 || valid.col > 8) return false;
    return true;
}

bool Board::board::checkmate(bool colour) const{
    //Checks if place colour is in checkmate.
    
    if(!this->check(colour)) return false; //Check if king is in check
    Code::squareCode checkPosition, kingPosition;
    kingPosition = this->findPiece(colour, 'K');
    //Check all legal moves the king can make, checks if in making them it moves out of check
    for(int rowNumb(-1); rowNumb <= 1; rowNumb++){
        for(int colNumb(-1); colNumb <=1; colNumb++){
            checkPosition.row = kingPosition.row + rowNumb;
            checkPosition.col = kingPosition.col + colNumb;
            if(validSquare(checkPosition)){
                if(! ( ( (*this)(checkPosition)->getPieceColour() == 'W' && colour) || ((*this)(checkPosition)->getPieceColour() == 'B' && !colour))){
                    if(!this->underAttack(colour, checkPosition)) return false;
                }
            }
        }
    }
    
    //Searches the board for all pieces belonging to colour
    //Once it finds one it searches the board for all spaces it can move to
    //If a legal move is found, the piece is moved there and the board is examined for check
    //If the final state of the board is no longer in check, the function returns false
    //Otherwise the piece is moved back to its original square
    //If no moves block the check then return true
    bool result(true);
    int rowOrigin(0), colOrigin(0);
    int rowBlock(0), colBlock(0);
    Code::squareCode position;
    Code::moveCode move;
    //Search board
    for_each(squarePoint->begin(), squarePoint->end(), [&](const std::vector<square> & row) ->bool{
        rowOrigin++;
        colOrigin = 0;
        for_each(row.begin(), row.end(), [&](const square & thisSquare)->bool{
            colOrigin++;
                if((thisSquare.getPieceColour() == 'W' && colour) || (thisSquare.getPieceColour() == 'B' && !colour)){
                    //Found a piece beloning to colour
                    rowBlock = 0;
                    colBlock = 0;
                    position.row = rowOrigin;
                    position.col = colOrigin;
                    move.initialSquare = position;
                    for_each(squarePoint->begin(), squarePoint->end(), [&](const std::vector<square> & blockRow) ->bool{
                        rowBlock++;
                        colBlock = 0;
                        for_each(blockRow.begin(), blockRow.end(), [&](const square & blockSquare)->bool{
                            colBlock++;
                            position.row = rowBlock;
                            position.col = colBlock;
                            move.finalSquare = position;
                            if((*blockSquare.getHomeBoard())(move.initialSquare)->checkBlockMove(colour, move)){
                                //Move blocks the check
                                result = false;
                            }
                            std::cout<< result << std::endl;
                            return result;
                        });
                        return result;
                    });
                }
            return result;
        });
        return result;
    });
    return result;
}








//  ############################################################################################
//  --------------------------------------------------------------------------------------------
//  ###################################  SQUARE MEMBER FUNCTION  ###############################
//  --------------------------------------------------------------------------------------------
//  ############################################################################################

//  Member function definitions for the square class.



square::square(const std::string pieceName, const bool inputColour, board* homeBoardInput){
    //Square constructor, construcs a square object with a piece specified by pieceName.
    //No piece is place if pieceName = "-"
    squareColour = inputColour;
    homeBoard = homeBoardInput;
    if(pieceName == "-") occupation = NULL;
    else{
        char pieceColour = pieceName[0];
        char pieceSide = pieceName[1];
        char pieceType = pieceName[2];
        
        if(pieceType == 'K') occupation = std::make_unique<king>(pieceColour);
        else if(pieceType == 'Q') occupation = std::make_unique<queen>(pieceColour, this);
        else if(pieceType == 'R') occupation = std::make_unique<rook>(pieceColour, pieceSide, this);
        else if(pieceType == 'B') occupation = std::make_unique<bishop>(pieceColour, pieceSide, this);
        else if(pieceType == 'N') occupation = std::make_unique<knight>(pieceColour, pieceSide, this);
        else if(pieceType == 'P') occupation = std::make_unique<pawn>(pieceColour, pieceSide, this);
        else exit(7); // Exit with error code 7 if pieces not labeled correctly.
        
    }
}

void square::printLine(const int line) const{
    //Prints a line in interface depiction of the square
    
    std::vector<std::string> output;
    char background, pieceColour;
    if(squareColour) background = '+'; //white squar background
    else background = '-'; //black square background
    
    if(occupation){
        if(occupation->getColour() == 'W') pieceColour = '#'; //white piece character
        else pieceColour = ' '; //Black piece character
        
        //Below are the interpace depcictions of the squares with pieces occupying them
        //1 pritns a background char
        //0 prints a piece colour char
        if(occupation->getType() == 'K'){
            output.push_back("11111111111111");
            output.push_back("11001100110011");
            output.push_back("11100000000111");
            output.push_back("11111000011111");
            output.push_back("11111000011111");
            output.push_back("11100000000111");
            output.push_back("11111111111111");
        }
        else if(occupation->getType() == 'Q'){
            output.push_back("11111111111111");
            output.push_back("11101011010111");
            output.push_back("11111000011111");
            output.push_back("11110000001111");
            output.push_back("11111000011111");
            output.push_back("11100000000111");
            output.push_back("11111111111111");
        }
        else if(occupation->getType() == 'R'){
            output.push_back("11111111111111");
            output.push_back("11100000000111");
            output.push_back("11111000011111");
            output.push_back("11111000011111");
            output.push_back("11111000011111");
            output.push_back("11100000000111");
            output.push_back("11111111111111");
        }
        else if(occupation->getType() == 'N'){
            output.push_back("11111111111111");
            output.push_back("11000000000011");
            output.push_back("11000111000111");
            output.push_back("11000111110111");
            output.push_back("11000001111111");
            output.push_back("11000000011111");
            output.push_back("11111111111111");
        }
        else if(occupation->getType() == 'B'){
            output.push_back("11111111111111");
            output.push_back("11111100111111");
            output.push_back("11111000011111");
            output.push_back("11110000001111");
            output.push_back("11111000011111");
            output.push_back("11110000001111");
            output.push_back("11111111111111");
        }
        else if(occupation->getType() == 'P'){
            output.push_back("11111111111111");
            output.push_back("11111111111111");
            output.push_back("11111100111111");
            output.push_back("11111000011111");
            output.push_back("11111000011111");
            output.push_back("11110000001111");
            output.push_back("11111111111111");
        }
        
        for(int i(0); i < 14; i ++){
            if(output[line][i]=='1') std::cout << background;
            else std::cout << pieceColour;
        }
    }
    
    else {
        //Blank square
        for(int i(0); i < 14; i++){
            std::cout << background;
        }
    }
}



void square::newPiece(char pieceType, char pieceColour, char pieceSide){
    //Puts a new pice on the square, used in pawn promotion
    if(pieceType == 'K') occupation = std::make_unique<king>(pieceColour);
    else if(pieceType == 'Q') occupation = std::make_unique<queen>(pieceColour, this);
    else if(pieceType == 'R') occupation = std::make_unique<rook>(pieceColour, pieceSide, this);
    else if(pieceType == 'B') occupation = std::make_unique<bishop>(pieceColour, pieceSide, this);
    else if(pieceType == 'N') occupation = std::make_unique<knight>(pieceColour, pieceSide, this);
    else if(pieceType == 'P') occupation = std::make_unique<pawn>(pieceColour, pieceSide, this);
    else exit(7);
}


bool square::castle(bool colour, Code::moveCode & move){
    //Checks if the castling criteria is met. If it is it invokes the castline memeber function in board
    if(homeBoard->check(colour)) throw 11;
    if(move.finalSquare.col == 3){
        //Left castle
        if((*homeBoard)(1, move.initialSquare.row)->getPiecePoint()->getType() == 'R' && !(*homeBoard)(move.initialSquare)->getPiecePoint()->getMoved()){
            //Checks if both rook and king haven't moved
            Code::squareCode castleThrough;
            castleThrough.row = move.finalSquare.row;
            castleThrough.col = 4;
            if(!homeBoard->underAttack(colour, castleThrough)){
                //Checks if castling through check
                if(homeBoard->underAttack(colour,move.finalSquare)) throw 11;//Cheks if castling into check
                std::cout<<"Castling\n";
                homeBoard->castle(move);
                return true;
            }
        }
    }
    if(move.finalSquare.col == 7){
        if((*homeBoard)(8, move.initialSquare.row)->getPiecePoint()->getType() == 'R' && !(*homeBoard)(move.initialSquare)->getPiecePoint()->getMoved()){
            Code::squareCode castleThrough;
            castleThrough.row = move.finalSquare.row;
            castleThrough.col = 6;
            if(!homeBoard->underAttack(colour, castleThrough)){
                 if(homeBoard->underAttack(colour,move.finalSquare)) throw 11;
                    std::cout<<"Castling\n";
                    homeBoard->castle(move);
                    return true;
            }
        }
    }
    return false;
}



void square::move(bool colour, Code::moveCode & move){
    //Takes an input of type moveCode, checks if the move is legal. If the move is legal it completes the move
    if(!getOccupation()) throw 4; //Check if there is piece on initial square
    if(getPiecePoint()->getColour() == 'B' && colour) throw 9; //Can't move opposition piece
    if(getPiecePoint()->getColour() == 'W' && !colour) throw 9; //Can't move opposition piece
    if((*homeBoard)(move.initialSquare)->getPiecePoint()->getType() == 'K' && !(*homeBoard)(move.initialSquare)->getPiecePoint()->getMoved()){
        //Checks is player is intending on castline
        if(this->castle(colour, move)) return;
    }
    if(!occupation->legalMove(move, false)) throw 6; //Check if move is legal
    if((*homeBoard)(move.finalSquare)->getPieceColour()==this->getPieceColour()) throw 5; //Check if taking own piece
    
    std::unique_ptr<piece> holdingPointer; //Makes a tempory unique pointer to hold a piece that has been taken while the board is examined to see if the player has moved into check
    
    if(getPiecePoint()->getType() == 'P'){
        if(abs(move.finalSquare.col - move.initialSquare.col) == 1 && move.finalSquare.row - move.initialSquare.row == colour){
            if((*this->getHomeBoard())(move.finalSquare.col, move.initialSquare.row)->getOccupation()){
                if((*this->getHomeBoard())(move.finalSquare.col, move.initialSquare.row)->getPiecePoint()->getType() == 'P'){
                    if((*this->getHomeBoard())(move.finalSquare.col, move.initialSquare.row)->getPiecePoint()->getMoved()){
                        holdingPointer = std::move((*this->getHomeBoard())(move.finalSquare.col, move.initialSquare.row)->getPiecePoint());
                        this->completeMove(move);//Complete the move
                        if(homeBoard->check(colour)){
                            //Undo move if in check after the move and throw error
                            Code::moveCode reverse;
                            reverse.finalSquare = move.initialSquare;
                            reverse.initialSquare = move.finalSquare;
                            (*homeBoard)(reverse.initialSquare)->completeMove(reverse);
                            (*homeBoard)(move.finalSquare.col, move.initialSquare.row)->setPiece(holdingPointer);
                            throw 10;
                        }
                        else holdingPointer.reset();
                        return;
                    }
                }
            }
        }
    }
    
    //If making a move that takes a piece hold taken piece, so it can be reinstated if check occurs
    if((*homeBoard)(move.finalSquare)->getOccupation()){
        holdingPointer = std::move((*homeBoard)(move.finalSquare)->getPiecePoint());
    }
    else holdingPointer = NULL;
    this->completeMove(move);//Complete the move
    if(homeBoard->check(colour)){
        //Undo move if in check after the move and throw error
        Code::moveCode reverse;
        reverse.finalSquare = move.initialSquare;
        reverse.initialSquare = move.finalSquare;
        (*homeBoard)(reverse.initialSquare)->completeMove(reverse);
        (*homeBoard)(move.finalSquare)->setPiece(holdingPointer);
        throw 10;
    }
    else holdingPointer.reset();
    
    
    if((*homeBoard)(move.finalSquare)->getPiecePoint()->getType() == 'K' || (*homeBoard)(move.finalSquare)->getPiecePoint()->getType() == 'R'){
        //Tracks if king or rook has moved for castling
        (*homeBoard)(move.finalSquare)->getPiecePoint()->switchMoved();
    }
    if((*homeBoard)(move.finalSquare)->getPiecePoint()->getType() == 'P'){
        //Tracks if king or rook has moved for castling
        if(colour && move.finalSquare.row == 4) (*homeBoard)(move.finalSquare)->getPiecePoint()->switchMoved();
        if(!colour && move.finalSquare.row == 5) (*homeBoard)(move.finalSquare)->getPiecePoint()->switchMoved();
    }
    
}

bool square::checkBlockMove(bool colour, Code::moveCode & move){
    //Checks if a move 'move' is legal and if it blocks check for side 'colour'
    //Returns board to its initial condition after checking
    bool result;
    if(getPiecePoint()->getColour() == 'B' && colour) return false; //Can't move opposition piece
    if(getPiecePoint()->getColour() == 'W' && !colour) return false;
    std::cout << "Own piece  ";
    if(!occupation->legalMove(move, true)) return false; //Check if move is legal
    std::cout << "Legal move ";
    if((*homeBoard)(move.finalSquare)->getPieceColour()==this->getPieceColour()) return false; //Check if taking own piece
    std::cout << "Empty/Opposition   ";
    //If making a move that takes a piece hold taken piece, so it can be reinstated when reversing the move
    std::unique_ptr<piece> holdingPointer;
    if((*homeBoard)(move.finalSquare)->getOccupation()){
        holdingPointer = std::move((*homeBoard)(move.finalSquare)->getPiecePoint());
    }
    else holdingPointer = NULL;
    this->completeMove(move);//Complete the move
    if(homeBoard->check(colour)) {result = false;} //Check if in check
    else {result = true;}
    
    //Reverse the move
    Code::moveCode reverse;
    reverse.finalSquare = move.initialSquare;
    reverse.initialSquare = move.finalSquare;
    (*homeBoard)(reverse.initialSquare)->completeMove(reverse);
    (*homeBoard)(move.finalSquare)->setPiece(holdingPointer);
    //std::cout << "Block result: " << result << "  ";
    std::cout << "      Block Result: " << result;
    return result;
}


void square::completeMove(Code::moveCode & move){
    //Completes a move specified by moveCode move.
    (*homeBoard)(move.initialSquare)->getPiecePoint()->setHomeSquare((*homeBoard)(move.finalSquare)); //Change homeSquare on moving piece
    (*homeBoard)(move.finalSquare)->setPiece(occupation); //Move unique pointer to new square
}











//  ############################################################################################
//  --------------------------------------------------------------------------------------------
//  ###################################  PIECE MEMBER FUNCTION  ###############################
//  --------------------------------------------------------------------------------------------
//  ############################################################################################

// Below are the legalMove function definitions for each of derived piece classes





// King, Queen, Rook, Bishop movement
bool linePiece::legalMove(const Code::moveCode & move, bool noThrow) const{
    //noThrow specifies an illegal move should cause and exception. Set to true no exception is throw,
    //this is the case when checking for blocka agains checkmate
    if(!distance){
        if(abs(move.finalSquare.col - move.initialSquare.col) <= 1 && abs(move.finalSquare.row - move.initialSquare.row) <= 1) return true;
        else return false;
    }
    if(!this->getHomeSquare()->getHomeBoard()->clearPath(move)){
        if(noThrow) return false;
        else throw 8;
        
    }
    if(straight && diagonal){
        if(move.finalSquare.col == move.initialSquare.col || move.finalSquare.row == move.initialSquare.row ||abs(move.finalSquare.col - move.initialSquare.col) == abs(move.finalSquare.row - move.initialSquare.row)) return true;
        else return false;
    }
    if(straight){
        if(move.finalSquare.col == move.initialSquare.col || move.finalSquare.row == move.initialSquare.row) return true;
        else return false;
    }
    if(diagonal){
        if(abs(move.finalSquare.col - move.initialSquare.col) == abs(move.finalSquare.row - move.initialSquare.row)) return true;
        else return false;
    }
    return false;
}

//Knight movement
bool knight::legalMove(const Code::moveCode & move, bool noThrow) const{
    if(abs(move.finalSquare.col - move.initialSquare.col) == 2 && abs(move.finalSquare.row - move.initialSquare.row) == 1) return true;
    if(abs(move.finalSquare.col - move.initialSquare.col) == 1 && abs(move.finalSquare.row - move.initialSquare.row) == 2) return true;
    else return false;
}


//Pawn movement
bool pawn::legalMove(const Code::moveCode & move, bool noThrow) const{
    if(move.finalSquare.col == move.initialSquare.col && (*this->getHomeSquare()->getHomeBoard())(move.finalSquare)->getOccupation()) return false;
    int dist;
    int colour;
    if(this->getColour()== 'W') colour = 1;
    else colour = -1;
    
    //Check if pawn hasn't moved yet so, so it has the option of moving two squares
    if((move.initialSquare.row == 2 && this->getColour() == 'W') || (move.initialSquare.row == 7 && this->getColour() == 'B')) {
        
        if(!this->getHomeSquare()->getHomeBoard()->clearPath(move)){
            if(noThrow) return false;
            else throw 8;
        }
        dist = 2;
    }
    else dist = 1;
    
    //Check if legal move
    if(move.finalSquare.col == move.initialSquare.col && colour*(move.finalSquare.row-move.initialSquare.row) <= dist && colour*(move.finalSquare.row-move.initialSquare.row ) > 0){
        //Promotion - checks if pawn reaches other end of the board
        if(colour==1){
            if(move.finalSquare.row == 8){
                if(noThrow) return true;
                else throw 7;
            }
        }
        if(colour==-1){
            if(move.finalSquare.row == 1){
                if(noThrow) return true;
                else throw 7;
            }
        }
        return true;
    }
    
    //Capturing pieces by diagonal movement
    if(abs(move.finalSquare.col - move.initialSquare.col) == 1 && move.finalSquare.row - move.initialSquare.row == colour){
        if((*this->getHomeSquare()->getHomeBoard())(move.finalSquare)->getOccupation()){
            if((*this->getHomeSquare()->getHomeBoard())(move.finalSquare)->getPiecePoint()->getColour() != this->getColour()){
                if(colour==1){
                    if(move.finalSquare.row == 8){
                        if(noThrow) return true;
                        else throw 7;
                    }
                }
                if(colour==-1){
                    if(move.finalSquare.row == 1){
                        if(noThrow) return true;
                        else throw 7;
                    }
                }
                return true;
            }
        }
    }
    
    //En Passant
     if(abs(move.finalSquare.col - move.initialSquare.col) == 1 && move.finalSquare.row - move.initialSquare.row == colour){
         if((*this->getHomeSquare()->getHomeBoard())(move.finalSquare.col, move.initialSquare.row)->getOccupation()){
             if((*this->getHomeSquare()->getHomeBoard())(move.finalSquare.col, move.initialSquare.row)->getPiecePoint()->getType() == 'P'){
                  if((*this->getHomeSquare()->getHomeBoard())(move.finalSquare.col, move.initialSquare.row)->getPiecePoint()->getMoved()){
                      return true;
                  }
             }
         }
    }
    return false;
}




