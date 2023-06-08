//
// Created by stefam on 14. 5. 2023.
//

#include "IGhostMovement.h"
#include <utility>
#include <deque>
#include <queue>
#include <climits>


void IGhostMovement::moveFromSpawn(Ghost &ghost) {
    pair<int, int> ghostPosition = ghost.getPosition();
    pair<int, int> navigatingTo = board.getBorderToCross();
    pair<int, int> oldSur = ghostPosition;
    pair<int, int> nextSur = ghostPosition;

    if (ghostPosition.first < navigatingTo.first)
        direction = Direction::Right;
    else if (ghostPosition.first > navigatingTo.first)
        direction = Direction::Left;
    else if (ghostPosition.second < navigatingTo.second)
        direction = Direction::Down;
    else if (ghostPosition.second > navigatingTo.second)
        direction = Direction::Up;

    nextSur = Ghost::getNextPosition(oldSur, direction);
    CellType nextCellType = board.at(nextSur);

    if (board.at(oldSur) == CellType::Border) {
        if (nextCellType == CellType::Spawn || nextCellType == CellType::Wall || nextCellType == CellType::Teleport) {
            // Vyber smer ak je stena
            vector<Direction> alternativeDirections;
            for (const auto &dir: AllDirections) {
                pair<int, int> newPos = Ghost::getNextPosition(ghostPosition, dir);
                CellType newPosType = board.at(newPos);
                if (board.isInArea(newPos) && newPosType != CellType::Spawn && newPosType != CellType::Wall &&
                    newPosType != CellType::Border) {
                    alternativeDirections.push_back(dir);
                }
            }
            // Nahodne vyber smer
            if (!alternativeDirections.empty()) {
                size_t randomIndex = rand() % alternativeDirections.size();
                direction = alternativeDirections[randomIndex];
                nextSur = Ghost::getNextPosition(ghostPosition, direction);
            } else {
                // Niet uteku
                return;
            }
            ghost.setIsAtHome(false);
        }
    } else {
        if (nextCellType == CellType::Wall) {
            // Vyber smer ak je stena
            vector<Direction> alternativeDirections;
            for (const auto &dir: AllDirections) {
                pair<int, int> newPos = Ghost::getNextPosition(ghostPosition, dir);
                CellType newPosType = board.at(newPos);
                if (board.isInArea(newPos) && newPosType != CellType::Wall) {
                    alternativeDirections.push_back(dir);
                }
            }
            // Nahodne vyber smer
            if (!alternativeDirections.empty()) {
                size_t randomIndex = rand() % alternativeDirections.size();
                direction = alternativeDirections[randomIndex];
                nextSur = Ghost::getNextPosition(ghostPosition, direction);
            } else {
                // Niet uteku
                return;
            }
        }
    }
    ghost.setPosition(nextSur);

}

void StraightXY::move(Ghost &ghost, pair<int, int> toPosition) {
    pair<int, int> ghostPosition = ghost.getPosition();
    pair<int, int> nextPos;

    vector<Direction> validDirections;
    Direction bestDirection = direction;
    int closestDistance = INT_MAX;

    for (const auto& dir : AllDirections) {
        nextPos = Ghost::getNextPosition(ghostPosition, dir);
        if (board.isInArea(nextPos) && board.at(nextPos) != CellType::Wall && board.at(nextPos) != CellType::Border) {
            int distanceToTarget = getDistance(nextPos, toPosition);
            if (distanceToTarget < closestDistance) {
                closestDistance = distanceToTarget;
                bestDirection = dir;
            }
            validDirections.push_back(dir);
        }
    }

    // If the Ghost gets stucked make him move him somewhere.
    if ((rand() % 100) < 15) {
        size_t randomIndex = rand() % validDirections.size();
        direction = validDirections[randomIndex];
    } else {
        direction = bestDirection;
    }

    // Move to the new position based on the chosen direction

    nextPos = Ghost::getNextPosition(ghostPosition, direction);
    if(board.at(nextPos)==CellType::Teleport){
        return;
    }
    ghost.setPosition(nextPos);

}


int StraightXY::getDistance(const pair<int, int> &first, const pair<int, int> &second) {
    return abs(first.first - second.first) + abs(first.second - second.second);
}




void DumbMovement::move(Ghost &ghost, pair<int,int> toPosition) {
    pair<int, int> ghostPosition = ghost.getPosition();
    pair<int, int> nextPos;

    vector<Direction> validDirections;
    for (const auto &dir: AllDirections) {
        nextPos = Ghost::getNextPosition(ghostPosition, dir);
        if (board.isInArea(nextPos) && board.at(nextPos) != CellType::Wall && board.at(nextPos) != CellType::Border && !hasBeenRecently(nextPos)) {
            validDirections.push_back(dir);
        }
    }

    if (validDirections.empty()) {
        recentPositions.clear();
        for (const auto &dir: AllDirections) {
            nextPos = Ghost::getNextPosition(ghostPosition, dir);
            if (board.isInArea(nextPos) && board.at(nextPos) != CellType::Wall && board.at(nextPos) != CellType::Border) {
                validDirections.push_back(dir);
            }
        }
    }
    // If ghost is entrapped it
    if (validDirections.empty()) {
        return;
    }

    // Choose one of the random viable directions.
    size_t randomIndex = rand() % validDirections.size();
    direction = validDirections[randomIndex];
    rememberPosition(ghostPosition); // Before moving to new position, remember the current one

    // Get the new position after choosing the direction and move the pacman if it isn't teleport
    nextPos = Ghost::getNextPosition(ghostPosition, direction);
    if(board.at(nextPos)==CellType::Teleport){
        return;
    }
    ghost.setPosition(nextPos);
}

void DumbMovement::rememberPosition(const pair<int, int> &pos) {
    if (recentPositions.size() >= 20) {  // check if the deque has 20 or more elements
        recentPositions.pop_front();  // remove the oldest element
    }
    recentPositions.push_back(pos);  // add the new position at the end
}

bool DumbMovement::hasBeenRecently(const pair<int, int> &pos) {
    return std::find(recentPositions.begin(), recentPositions.end(), pos) != recentPositions.end();
}


void SmartMovement::move(Ghost &ghost, pair<int,int> toPosition) {
    std::queue<std::pair<int, int>> q;
    std::map<std::pair<int, int>, std::pair<int, int>> prev;

    auto start = ghost.getPosition();

    q.push(start);

    while(!q.empty()) {
        pair<int,int> curr = q.front();
        q.pop();

        if(curr == toPosition) {
            std::vector<std::pair<int, int>> path;

            for(auto i = curr; i != start; i = prev[i]) {
                path.push_back(i);
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());

            if(path.size() > 1) {
                auto nextPosition = path[1];
                if(nextPosition.first < start.first)
                    direction = Direction::Left;
                else if(nextPosition.first > start.first)
                    direction = Direction::Right;
                else if(nextPosition.second < start.second)
                    direction = Direction::Up;
                else if(nextPosition.second > start.second)
                    direction = Direction::Down;

                if(board.at(nextPosition)==CellType::Teleport){
                    return;
                }
                ghost.setPosition(nextPosition);
            }
            return;
        }

        for(const auto &dir : AllDirections) {
            auto nextPos = Ghost::getNextPosition(curr, dir);
            if (board.isInArea(nextPos) && board.at(nextPos) != CellType::Wall && board.at(nextPos) != CellType::Border && prev.count(nextPos) == 0) {
                q.push(nextPos);
                prev[nextPos] = curr;
            }
        }
    }
}



