//
//  main.cpp
//  chessMaster
//
//  Created by Karan Mukhi on 08/04/2019.
//  Copyright © 2019 Karan Mukhi. All rights reserved.
//  Main program for a chess game program written in c++
//  Program welcomes and initialsises a game
//  Then lets users play a game of chess
//  After the game is concluded players are given the option to qutit or play again

#include <iostream>
#include <memory>
#include "game.hpp"
using namespace std;

void printTitle(){
    std::cout << " ____      ____      __" << std::endl;
    std::cout << "|_  _|    |_  _|    [  |" << std::endl;
    std::cout << "  \\ \\  /\\  / /.---.  | |  .---.   .--.   _ .--..--.  .---." << std::endl;
    std::cout << "   \\ \\/  \\/ // /__\\\\ | | / /'`\\]/ .'`\\ \\[ `.-. .-. |/ /__\\\\" << std::endl;
    std::cout << "  _ \\  /\\  / | \\__., | | | \\__. | \\__. | | | | | | || \\__.," << std::endl;
    std::cout << " / |_\\/  \\/   '.__.'[___]'.___.' '.__.' [___||__||__]'.__.'" << std::endl;
    std::cout << "`| |-' .--.          ______  __"<< std::endl;
    std::cout << " | | / .'`\\ \\      .' ___  |[  |"<< std::endl;
    std::cout << " | |,| \\__. |     / .'   \\_| | |--.  .---.  .--.   .--."<< std::endl;
    std::cout << " \\__/ '.__.'      | |        | .-. |/ /__\\\\( (`\\] ( (`\\]"<< std::endl;
    std::cout << "                  \\ `.___.'\\ | | | || \\__., `'.'.  `'.'."<< std::endl;
    std::cout << "                   `.____ .'[___]|__]'.__.'[\\__) )[\\__) )\n\n\n\n\n";
    
    
}

int main() {
    
    printTitle();
    string input;
    while(true){
        game chess;
        chess.play();
        cout << "Would you like to play again?\n";
        std::getline(std::cin, input);
        if(!(input[0] == 'y' || input[0] =='Y')) break;
    }
    cout << "Goodbye\n";
    
    return 0;
}


