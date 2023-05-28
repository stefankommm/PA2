#include <iostream>

#include "Game.h"


#include "rang.hpp"

using namespace rang;
using namespace std;

int main() {
//    system("gnome-terminal -- sh -c './Semestralka_PA2'");
//
//    // Redirect error output to the new terminal
//    std::cerr.rdbuf(nullptr); // Disable default error output
//    std::cerr.rdbuf(std::cout.rdbuf()); // Redirect error output to standard output
    std:cerr << "BRUH" << endl;
    Game game;
    game.run();
    return 0;
}