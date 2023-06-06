//
// Created by stefam on 14. 5. 2023.
//

#include "IGhostMovement.h"
#include <utility>
#include <deque>


pair<int, int> getNextPosition(const pair<int, int> &position, Direction direction) {
    int newRow = position.second;
    int newCol = position.first;

    switch (direction) {
        case Direction::Up:
            newRow--;
            break;
        case Direction::Down:
            newRow++;
            break;
        case Direction::Left:
            newCol--;
            break;
        case Direction::Right:
            newCol++;
            break;
    }
    return make_pair(newCol, newRow);
}


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

    nextSur = getNextPosition(oldSur, direction);
    CellType nextCellType = board.at(nextSur);

    if (board.at(oldSur) == CellType::Border) {
        if (nextCellType == CellType::Spawn || nextCellType == CellType::Wall) {
            // Vyber smer ak je stena
            vector<Direction> alternativeDirections;
            for (const auto &dir: AllDirections) {
                pair<int, int> newPos = getNextPosition(ghostPosition, dir);
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
                nextSur = getNextPosition(ghostPosition, direction);
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
                pair<int, int> newPos = getNextPosition(ghostPosition, dir);
                CellType newPosType = board.at(newPos);
                if (board.isInArea(newPos) && newPosType != CellType::Wall) {
                    alternativeDirections.push_back(dir);
                }
            }
            // Nahodne vyber smer
            if (!alternativeDirections.empty()) {
                size_t randomIndex = rand() % alternativeDirections.size();
                direction = alternativeDirections[randomIndex];
                nextSur = getNextPosition(ghostPosition, direction);
            } else {
                // Niet uteku
                return;
            }
        }
    }
        ghost.setPosition(nextSur);

}


void RandomMovement::move(Ghost &ghost) {

}

void DumbMovement::move(Ghost &ghost) {
    pair<int, int> ghostPosition = ghost.getPosition();
    pair<int, int> nextPos;
    CellType nextPosType;

    vector<Direction> validDirections;
    for (const auto &dir: AllDirections) {
        nextPos = getNextPosition(ghostPosition, dir);
        if (board.isInArea(nextPos) && board.at(nextPos) != CellType::Wall && board.at(nextPos) != CellType::Border && !hasBeenRecently(nextPos)) {
            validDirections.push_back(dir);
        }
    }

    // if there's any valid direction, choose one at random
    if (!validDirections.empty()) {
        size_t randomIndex = rand() % validDirections.size();
        direction = validDirections[randomIndex];
    } else {
        // If there is no valid direction - remove cache and find direction where it is possible to pass
        recentPositions.clear();

        for (const auto &dir: AllDirections) {
            nextPos = getNextPosition(ghostPosition, dir);
            rememberPosition(nextPos);
            CellType onNext = board.at(nextPos);
            if (onNext != CellType::Wall && onNext != CellType::Border && onNext != CellType::Teleport) {
                direction = dir;
            }
        }
    }

    // get the new position after choosing the direction
    nextPos = getNextPosition(ghostPosition, direction);
    CellType nextCellType = board.at(nextPos);

    // re-checking that next position is valid
    if (nextCellType == CellType::Wall || nextCellType == CellType::Border) {
        // if there are no valid directions, reset the ghost's memory and pick a random direction
        recentPositions.clear();
        size_t randomIndex = rand() % AllDirections.size();
        direction = AllDirections[randomIndex];
        // get the new position after choosing the direction
        nextPos = getNextPosition(ghostPosition, direction);
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


void SmartMovement::move(Ghost &ghost) {
    pair<int, int> ghostPosition = ghost.getPosition();
    pair<int, int> navigatingTo = board.getPacmanPosition();
    pair<int, int> nextSur;

    if (ghostPosition.first < navigatingTo.first)
        direction = Direction::Right;
    else if (ghostPosition.first > navigatingTo.first)
        direction = Direction::Left;
    else if (ghostPosition.second < navigatingTo.second)
        direction = Direction::Down;
    else if (ghostPosition.second > navigatingTo.second)
        direction = Direction::Up;
    else {
        return;
    }

    nextSur = getNextPosition(ghostPosition, direction);

    if (board.isInArea(nextSur)) {
        CellType nextCellType = board.at(nextSur);
        if (nextCellType == CellType::Wall || nextCellType == CellType::Border || nextCellType == CellType::Teleport) {
            // Vyber smer ak je stena
            vector<Direction> alternativeDirections;
            for (const auto &dir: AllDirections) {
                pair<int, int> newPos = getNextPosition(ghostPosition, dir);
                CellType newPosType = board.at(newPos);
                if (board.isInArea(newPos) && newPosType != CellType::Wall && newPosType != CellType::Border && newPosType != CellType::Teleport) {
                    alternativeDirections.push_back(dir);
                }
            }
            // Nahodne vyber smer
            if (!alternativeDirections.empty()) {
                size_t randomIndex = rand() % alternativeDirections.size();
                direction = alternativeDirections[randomIndex];
                nextSur = getNextPosition(ghostPosition, direction);
            } else {
                // Niet uteku
                return;
            }
        }

        ghost.setPosition(nextSur);

    }
}



