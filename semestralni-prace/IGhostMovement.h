//
// Created by stefam on 14. 5. 2023.
//

#pragma once


#include <utility>
#include "Character.h"
#include "Board.h"

class Ghost;
class Board;

class IGhostMovement {
public:
    virtual ~IGhostMovement() = default;

    // The ghost and the board are passed as parameters to the move method.
    virtual void move(Ghost& ghost, const std::unique_ptr<Board>& boardPtr) = 0;

protected:
    int speed;
};

class SmartMovement : public IGhostMovement {
public:
    void move(Ghost& ghost, const std::unique_ptr<Board>& boardPtr) override;
};

class RandomMovement : public IGhostMovement {
public:
    void move(Ghost& ghost, const std::unique_ptr<Board>& boardPtr) override;
};

class DumbMovement : public IGhostMovement {
public:
    void move(Ghost& ghost, const std::unique_ptr<Board>& boardPtr) override;
};


