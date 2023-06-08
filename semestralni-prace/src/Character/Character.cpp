//
// Created by stefam on 14. 5. 2023.
//
#include "Character.h"


pair<int, int> Character::getPosition() const {
    return {xSur, ySur};
}

void Character::setPosition(pair<int, int> newPos) {
    xSur = newPos.first;
    ySur = newPos.second;
}

void Character::respawn() {
    this->setPosition(spawnPoint);
}

bool Character::isAlive() const {
    return alive;
}

bool Character::isReverseEating() const {
    return reverseEating;
}


void Pacman::setDirection(Direction dir) {
    direction = dir;
}

Direction Pacman::getDirection() const {
    return direction;
}

char Pacman::getSymbol() const {
    return directionSymbols.at(direction);
}

void Pacman::move() {
    // UPDATE PACMAN POSITION;
    pair<int, int> oldSur = getPosition();
    pair<int, int> nextSur = oldSur;
    switch (getDirection()) {
        case Direction::Up:
            nextSur.second--;
            break;
        case Direction::Down:
            nextSur.second++;
            break;
        case Direction::Left:
            nextSur.first--;
            break;
        case Direction::Right:
            nextSur.first++;
            break;
    };


    if (board.isInArea(nextSur)) {
        switch (board.at(nextSur)) {
            case CellType::Wall:
            case CellType::Border:
                break;
            case CellType::Teleport:
            case CellType::Empty:
            default:
                setPosition(nextSur);
                break;
        }
    } else {
        pair<int, int> newSur = board.getTeleports().at(make_pair(oldSur.first, oldSur.second));
        setPosition(newSur);
    }
}


Pacman::Pacman(int x, int y, Board &board) :
    Character(x, y, board),
    direction(Direction::Right) {}

void Pacman::setReverseEating(bool state) { reverseEating = state; };

char Ghost::getSymbol() const {
    return DEFAULT_GHOST_SYMBOL;
}

void Ghost::move() {
    if (isAtHome) {
        movementStrategy->moveFromSpawn(*this);
    } else {
        if (isReverseEating()) {
            movementStrategy->move(*this, board.getFarthestCoordinatesFromPacman());
        } else {
            movementStrategy->move(*this, board.getPacmanPosition());
        }
    }
}

void Ghost::setReverseEating(bool state) {
    this->reverseEating = state;

}

void Ghost::respawn() {
    this->setReverseEating(false);
    this->setPosition(spawnPoint);
    isAtHome = true;
}

void Ghost::setIsAtHome(bool state) {
    isAtHome = state;
}



