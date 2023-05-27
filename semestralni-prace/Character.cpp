//
// Created by stefam on 14. 5. 2023.
//

#include "Character.h"


void Character::setReverseEating(bool state) {
    reverseEating = true;
}

Character::Character(int x, int y) {

}

void Character::move(int dx, int dy) {

}

char Character::getSymbol() const {
    return 0;
}


void Pacman::setDirection(Direction dir) {
    direction = dir;

}

Direction Pacman::getDirection() {
    return Direction::Left;
}

char Pacman::getSymbol() const {
    return '<';
}

char Pacman::getSymbol(Direction dir) const {
    return directionSymbols.at(dir);
}

char Ghost::getSymbol() const {
    return 'G';
}


