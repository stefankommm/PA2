//
// Created by stefam on 14. 5. 2023.
//
#pragma once

#include <memory>
#include "IGhostMovement.h"
#include "Board.h"
#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <map>


class IGhostMovement;
enum class Direction {
    Up,
    Down,
    Left,
    Right
};

class Character {
public:
    Character(int x, int y);
    virtual void move(int dx, int dy);
    virtual char getSymbol() const;
    void setReverseEating(bool state);

protected:
    int positionX, positionY;
    bool reverseEating;
    Direction direction;
};

class Pacman : public Character {
public:
    Pacman(int x, int y);
    char getSymbol() const override;
    char getSymbol(Direction dir) const; //pacman refrenced from pointer
    void setDirection(Direction dir);
    Direction getDirection();

private:
    std::map<Direction, char> directionSymbols {
            {Direction::Up, '^'},
            {Direction::Down, 'v'},
            {Direction::Left, '<'},
            {Direction::Right, '>'}
    };};


class Ghost : public Character {
public:
    Ghost(int x, int y, std::unique_ptr<IGhostMovement> movementStrategy);

    void move(const Board& board);

    char getSymbol() const override;

private:
    std::pair<int,int> nextPosition;
    std::unique_ptr<IGhostMovement> movementStrategy;

};


