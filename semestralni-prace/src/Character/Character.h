//
// Created by stefam on 14. 5. 2023.
//
#pragma once

#include <memory>
#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <map>

#include "IGhostMovement.h"
#include "constants.h"
class Board;

using namespace std;

class IGhostMovement;


class Character {
public:
    Character(int x, int y, Board &board) :
            xSur(x),
            ySur(y),
            board(board),
            spawnPoint({x, y}),
            reverseEating(false),
            alive(true)
            {};
    virtual ~Character() = default;
    virtual void move() = 0;
    virtual char getSymbol() const = 0;
    virtual void setReverseEating(bool state) = 0;
    bool isAlive() const;
    bool isReverseEating() const;
    virtual void respawn();
    pair<int, int> getPosition() const;
    void setPosition(pair<int,int>);


protected:
    Board &board;
    int xSur{}, ySur{};
    pair<int, int> spawnPoint;
    bool alive{};
    bool reverseEating;
};


class Pacman : public Character {
public:
    Pacman(int x, int y, Board &board);
    [[nodiscard]] char getSymbol() const override;
    void move() override;
    void setDirection(Direction dir);
    [[nodiscard]] Direction getDirection() const;
    void setReverseEating(bool state) override;

private:
    Direction direction;
    std::map<Direction, char> directionSymbols{
            {Direction::Up,    '^'},
            {Direction::Down,  'v'},
            {Direction::Left,  '<'},
            {Direction::Right, '>'}
    };
};


class Ghost : public Character {
public:
    Ghost(int x, int y, Board &board, unique_ptr<IGhostMovement> movementStrategy) :
        Character(x,y, board),
        movementStrategy(std::move(movementStrategy)),
        isAtHome(true),
        alive(true)
        {};

    char getSymbol() const override;
    void move() override;
    void respawn() override;
    void setIsAtHome(bool state);
    void setReverseEating(bool state) override;


private:
    bool isAtHome;
    bool alive;
    std::unique_ptr<IGhostMovement> movementStrategy;
};


