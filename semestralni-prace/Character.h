//
// Created by stefam on 14. 5. 2023.
//
#pragma once

#include <memory>
#include "IGhostMovement.h"
#include "Board.h"
#include "config.h"
#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <map>
using namespace std;



class IGhostMovement;


class Character {
public:
    Character(int x, int y, Board & board) : xSur(x), ySur(y), board(board) {};
    virtual ~Character() = default;
    virtual void move() = 0;
    virtual char getSymbol() const = 0;
    virtual void setReverseEating(bool state) = 0;
    pair<int,int> getPosition() const;
    void setPosition(int x, int y);
protected:
    Board & board;
    int xSur{}, ySur{};
    bool reverseEating{};
};

class Pacman : public Character {
public:
    Pacman(int x, int y, Board & board) : Character(x, y, board), direction(Direction::Right) {};
    char getSymbol() const override;
    void move() override;
    void setDirection(Direction dir);
    Direction getDirection() const;
    void setReverseEating(bool state) override;


private:
    Direction direction;
    std::map<Direction, char> directionSymbols {
            {Direction::Up, '^'},
            {Direction::Down, 'v'},
            {Direction::Left, '<'},
            {Direction::Right, '>'}
    };};


class Ghost : public Character {
public:
    Ghost(int x, int y, Board& board,std::unique_ptr<IGhostMovement> movementStrategy);
    void move() override{};
    char getSymbol() const override;

private:
    Board& board;
    std::pair<int,int> nextPosition;
    std::unique_ptr<IGhostMovement> movementStrategy;

};


