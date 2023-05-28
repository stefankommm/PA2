//
// Created by stefam on 14. 5. 2023.
//

#include "Character.h"


pair<int, int> Character::getPosition() const {
    return {xSur, ySur};
}

void Character::setPosition(int x, int y) {
    xSur = x;
    ySur = y;
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
    }
    if(nextSur.first >= 0 && nextSur.first < board.grid.size() &&
       nextSur.second >= 0 && nextSur.second < board.grid.size()) {
        if (board.grid[nextSur.first][nextSur.second] != CellType::Wall &&
            board.grid[nextSur.first][nextSur.second] != CellType::Border) {
            setPosition(nextSur.first, nextSur.second);
            board.grid[oldSur.first][oldSur.second] = CellType::Empty;
            board.grid[nextSur.first][nextSur.second] = CellType::Pacman;
        }
    }
}

void Pacman::setReverseEating(bool state) {

}





//Ghost::Ghost(int x, int y, Board &board, std::unique_ptr<IGhostMovement> strategy) : Character(x,y), board(board), movementStrategy(move(strategy)) {
//
//}
char Ghost::getSymbol() const {
    return 0;
}
