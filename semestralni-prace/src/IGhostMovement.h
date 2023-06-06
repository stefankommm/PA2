//
// Created by stefam on 14. 5. 2023.
//

#pragma once


#include <utility>
#include <deque>
#include <utility>

#include "Character/Character.h"
#include "Board/Board.h"
#include "constants.h"


using namespace std;

class Ghost;
class Board;


class IGhostMovement { ;
public:
    explicit IGhostMovement(Board &board) : board(board), direction(Direction::Right) {};
    virtual ~IGhostMovement() = default;
    virtual void move(Ghost &ghost) = 0;
    void moveFromSpawn(Ghost &ghost);

protected:
    Direction direction;
    Board &board;
};

class SmartMovement : public IGhostMovement {
public:
    explicit SmartMovement(Board &board) : IGhostMovement(board) {}
    void move(Ghost &ghost) override;

private:
};

class RandomMovement : public IGhostMovement {
public:
    explicit RandomMovement(Board &board) : IGhostMovement(board) {}
    void move(Ghost &ghost) override;
};

class DumbMovement : public IGhostMovement {
public:
    explicit DumbMovement(Board &board) : IGhostMovement(board) {}
    void move(Ghost &ghost) override;
    void rememberPosition(const pair<int, int> &pos);
    bool hasBeenRecently(const pair<int, int> &pos);
private:
    std::deque<pair<int, int>> recentPositions;
};


