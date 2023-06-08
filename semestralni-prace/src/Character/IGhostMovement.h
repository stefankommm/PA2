//
// Created by stefam on 14. 5. 2023.
//

#pragma once

#include <utility>
#include <deque>
#include <utility>
#include <stack>

#include "Character/Character.h"


using namespace std;

class Ghost;
class Board;

/**
 * @brief Abstract class which abstracts the different types of the Ghosts movement
 *
 */
class IGhostMovement {
public:
    explicit IGhostMovement(Board &board) : direction(Direction::Right), board(board) {};
    virtual ~IGhostMovement() = default;
    virtual void move(Ghost &ghost, pair<int,int> toPosition) = 0;

    /**
    * Function which handles navigating Each Ghost from the Spawn. Navigates the Ghost
    * to the first defined border.
    * @param ghost Reference to the ghost which is being navigated
    */
    void moveFromSpawn(Ghost &ghost);

protected:
    Direction direction;
    Board &board;
};

/**
 * @brief BFS navigation for the ghost
 * Navigation for ghost which finds the shortest path to the toPosition using BFS algorithm.
 * Updates the ghosts coordinate based on the calculated path. If the next position is a teleport
 * it ignores it
 */
class SmartMovement : public IGhostMovement {
public:
    explicit SmartMovement(Board &board) : IGhostMovement(board) {}

    /**
     * Navigation for ghost which finds the shortest path to the toPosition using BFS algorithm.
     * Updates the ghosts coordinate based on the calculated path. If the next position is a teleport
     * it ignores it
     * @param ghost Reference to the ghost which is beeing moved.
     * @param toPosition Position the ghost is navigated to
     */
    void move(Ghost &ghost, pair<int,int> toPosition) override;
};


/**
 *  @brief Navigation for the ghost which navigates the ghost to the tile which is closest to the pacman.
    * Navigation which prefers to go to the place which is closer to the (Pacman/Furhest from pacman). It calculates distance for
    * each direction and navigates 65 percent to this place. in 35% the prediction is random.
   */
class StraightXY : public IGhostMovement {
public:
    explicit StraightXY(Board &board) : IGhostMovement(board) {}
    /**
     * Navigation which prefers to go to the place which is closer to the (Pacman/Furhest from pacman). It calculates distance for
     * each direction and navigates 65 percent to this place. in 15% the prediction is random.
     * @brief Function that tries to navigate the Ghost way the nextCoordinate would be closer to the toPosition with some randomness
     * @param ghost Ghost reference on which the movement is done
     * @param toPosition Position where the pacman should ideally end in (Pacman's position/Position furthest from Pacman)
    */
    void move(Ghost &ghost, pair<int,int> toPosition) override;
    static int getDistance(const pair<int, int> &first, const pair<int, int> &second);
};
/**
 * @brief Navigation which moves the ghost to the coordinate which is closest to the pacman
 * Navigation which moves Ghost randomly on the position which isn't stored in recentPositions.
 * If the Ghost gets into the path where there is only one way out it clears the recentPositions.
 */
class DumbMovement : public IGhostMovement {
public:
    explicit DumbMovement(Board &board) : IGhostMovement(board) {}
    /**
     * Navigation which moves Ghost randomly on the position which isn't stored in recentPositions.
     * If the Ghost gets into the path where there is only one way out it clears the recentPositions.
     * @param ghost Ghost reference on which the movement is done
     * @param toPosition Position where the pacman should ideally end in (Pacman's position/Position furthest from Pacman)
     */
    void move(Ghost &ghost, pair<int,int> toPosition) override;
    void rememberPosition(const pair<int, int> &pos);
    bool hasBeenRecently(const pair<int, int> &pos);
private:
    std::deque<pair<int, int>> recentPositions;
};


