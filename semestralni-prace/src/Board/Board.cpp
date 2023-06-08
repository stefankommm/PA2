//
// Created by stefam on 14. 5. 2023.
//

#include <queue>
#include "Board.h"
#include "Configuration.h"


Board::Board(std::vector<std::vector<CellType>> initialGrid, LevelSettings settings)
        : grid(std::move(initialGrid)),
          ticksTillReverseEnds(0),
          updateNext(false),
          reverseEating(false),
          reverseEatingEated(0),
          coinsToCollect(0),
          coinsCollected(0),
          score(0),
          lives(DEFAULT_LIVES),
          pacman_ptr(nullptr),
          REVERSE_EATING_LENGTH(settings.reverseEatingLength),
          PACMAN_SPEED(settings.pacmanSpeed),
          GHOST_SPEED(settings.ghostSpeed),
          LCM_TICK(calculateLCM(settings.pacmanSpeed, settings.ghostSpeed)) {
    initialize(settings.ghosts);
};


void Board::initialize(const std::vector<std::string> &ghostsToCreate) {
    int pacmanSpawns = 0;
    int ghostSpawns = 0;
    try {
        for (int x = 0; x < (int) grid.size(); x++) {
            for (int y = 0; y < (int) grid[0].size(); y++) {
                switch (grid[x][y]) {
                    case CellType::CoinPoints:
                    case CellType::CoinReverseEating:
                        coinsToCollect++;
                        break;
                    case CellType::GhostSpawn:
                        createGhost(x, y, ghostsToCreate[ghostSpawns % ghostsToCreate.size()]);
                        grid[x][y] = CellType::Empty;
                        ghostSpawns += 1;
                        break;
                    case CellType::PacmanSpawn:
                        if (pacmanSpawns >= 1)
                            throw std::runtime_error("Na ploche nemoze byt viac ako jeden pacman");
                        grid[x][y] = CellType::Empty;
                        createPacman(x, y);
                        pacmanSpawns += 1;
                        break;
                    case CellType::Teleport:
                        //TODO: TELEPORT
                        addTeleport({x, y});
                        break;
                    case CellType::Border:
                        borderToCross = {x, y};
                        break;
                    default:
                        break;
                }
            }
        }
    }
    catch (int s) {
        cout << "NIECO ZLE";
    }
    if (pacmanSpawns == 0) {
        throw std::runtime_error("Mapa musi obsahovat Spawn pre PACMANA - 'P' ");
    }
    if (ghostSpawns == 0) {
        throw std::runtime_error("Mapa musi obsahovat aspon jeden spawn pre Ducha - 'G' ");
    }
}


void Board::rotatePacman(Direction dir) {
    pacman_ptr->setDirection(dir);

}


int Board::isGhostPosition(int xSur, int ySur) const {
    int pos = 0;
    for (const auto &g: ghosts) {
        pair<int, int> position = g->getPosition();
        if (position.first == xSur && position.second == ySur) {
            return pos;
        }
        pos++;
    };
    return -1;
}

/**
 * @brief Updates the positions of the pacman and ghosts on the board. Checks if the pacman and ghost collided
 *
 * This function is called to update the positions of the pacman and ghosts on the board based on the specified tick.
 * It checks for collisions between the pacman and ghosts, handles the collisions accordingly, and updates the positions
 * accordingly.
 *
 * @param tick The current tick value used to determine the update interval.
 */
void Board::updatePositions(unsigned long long tick) {
    updateNext = false;

    checkEatenCoins();


    if (tick % LCM_TICK == 0) {
        checkColissions(false);
        updateNext = true;
        vector<pair<int, int>> prevGhostPositions{};
        vector<pair<int, int>> updatedGhostPositions{};
        pair<int, int> prevPacmanPosition = pacman_ptr->getPosition();
        pair<int, int> updatedPacmanPosition;

        for (const auto &ghost: ghosts) {
            prevGhostPositions.push_back(ghost->getPosition());
        }

        movePacman();
        moveGhosts();

        updatedPacmanPosition = pacman_ptr->getPosition(); // Assign the updated pacman position here

        for (const auto &ghost: ghosts) {
            updatedGhostPositions.push_back(ghost->getPosition());
        }

        for (size_t i = 0; i < ghosts.size(); i++) {
            if (prevGhostPositions[i] == updatedPacmanPosition &&
                updatedGhostPositions[i] == prevPacmanPosition) {
                if (ghosts[i]->isReverseEating()) {
                    reverseGhostEaten(*ghosts[i]);
                } else {
                    pacmanEaten();
                }
            }
        }
    } else {
        if (tick % GHOST_SPEED == 0) {
            checkColissions(true);
            moveGhosts();
            updateNext = true;
        } else if (tick % PACMAN_SPEED == 0) {
            checkColissions(false);
            movePacman();
            updateNext = true;
        }
    }
}


pair<int, int> Board::getPacmanPosition() const {
    return pacman_ptr->getPosition();
}

void Board::createPacman(int x, int y) {
    pacman_ptr = make_unique<Pacman>(x, y, *this);
}


CellType Board::at(pair<int, int> check) const {
    if (this->isInArea(check)) {
        return grid[check.first][check.second];
    }
    return CellType::Empty;
}

bool Board::isInArea(pair<int, int> check) const {
    return (check.first >= 0 && check.first < (int) grid.size() &&
            check.second >= 0 && check.second < (int) grid[0].size());
}

void Board::respawnPacman() {
    pacman_ptr->respawn();
}

void Board::respawnGhosts() {
    for (auto &i: ghosts) {
        i->respawn();
    }
}

void Board::respawnAll() {
    respawnPacman();
    respawnGhosts();
}

void Board::eatCoinPoints() {
    score += 20;
    coinsCollected++;
}

void Board::eatCoinReverseEating() {
    enableReverseEating();
    coinsCollected++;
}

void Board::pacmanEaten() {
    lives--;
    disableReverseEating();
    respawnAll();
}

int Board::getScore() const {
    return score;
}

bool Board::isEnd() const {
    return !(coinsToCollect != coinsCollected && lives > 0);
}


pair<int, int> Board::getBorderToCross() const {
    return borderToCross;
}

bool Board::shouldUpdate() const {
    return updateNext;
}

void Board::createGhost(int x, int y, const string &type) {
    if (type == "SMART") {
        std::unique_ptr<IGhostMovement> smartMovement = std::make_unique<SmartMovement>(*this);
        ghosts.emplace_back(std::make_unique<Ghost>(x, y, *this, std::move(smartMovement)));
    } else if (type == "STRAIGTXY") {
        std::unique_ptr<IGhostMovement> randomMovement = std::make_unique<StraightXY>(*this);
        ghosts.emplace_back(std::make_unique<Ghost>(x, y, *this, std::move(randomMovement)));
    } else if (type == "DUMB") {
        std::unique_ptr<IGhostMovement> dumbMovement = std::make_unique<DumbMovement>(*this);
        ghosts.emplace_back(std::make_unique<Ghost>(x, y, *this, std::move(dumbMovement)));
    } else {
        throw std::invalid_argument("There is no a ghost " + type + " Avaialable ghosts are: SMART, STRAIGHTXY, DUMB");
    }
}


bool Board::isReverseEating() const {
    return reverseEating;
}

void Board::disableReverseEating() {
    reverseEating = false;
    reverseEatingEated = 0;
    for (auto &g: ghosts) {
        g->setReverseEating(false);
    }
}

void Board::enableReverseEating() {
    reverseEating = true;
    reverseEatingEated = 0;
    ticksTillReverseEnds = (int) REVERSE_EATING_LENGTH * DEFAULT_TICKS_SECOND;
    for (auto &g: ghosts) {
        if (g->isAlive()) {
            g->setReverseEating(true);
        }
    }
}

void Board::reverseGhostEaten(Character &ghost) {
    reverseEatingEated++;
    ghost.respawn();
    score += (int) std::pow(2, reverseEatingEated) * 100;
}

void Board::checkColissions(bool checkTeleports) {
    pair<int, int> pacmanPosition = pacman_ptr->getPosition();
    vector<Character *> ghostsStanding;

    if(checkTeleports){
        vector<Character *> closeToThePacmanNearTeleport;
        for (auto &ghost : ghosts) {
            for (const auto &dir : AllDirections) {
                pair<int, int> nextPos = ghost->getNextPosition(ghost->getPosition(), dir);
                if (nextPos == pacmanPosition) {
                    closeToThePacmanNearTeleport.push_back(ghost.get());
                    break;
                }
            }
        }
        for(const auto & g : closeToThePacmanNearTeleport){
            if(g->isReverseEating()){
                reverseGhostEaten(*g);
            } else {
                pacmanEaten();
                return;
            }
        }

    }


    for (unique_ptr<Character> &ghost: ghosts) {
        if (ghost->getPosition() == pacmanPosition) {
            ghostsStanding.push_back(ghost.get());
        }
    }
    if (ghostsStanding.empty()) {
        return;
    }
    for (auto &ghost: ghostsStanding) {
        if (ghost->isReverseEating()) {
            reverseGhostEaten(*ghost);
        } else {
            pacmanEaten();
            break;
        }
    }
}


int Board::calculateLCM(int a, int b) {
    int hcf = std::__gcd(a, b);
    int lcm = std::abs(a * b) / hcf;
    return lcm;
}

void Board::moveGhosts() {
    for (auto &pac: ghosts) {
        pac->move();
    }
}

void Board::setTicksTillReverseEnds(int toSet) {
    Board::ticksTillReverseEnds = toSet;
}

void Board::movePacman() {
    pacman_ptr->move();
}

int Board::getTicksTillReverseEnds() const {
    return ticksTillReverseEnds;
}

int Board::getReverseEatingEated() const {
    return reverseEatingEated;
}

int Board::getCoinsToCollect() const {
    return coinsToCollect;
}

int Board::getCoinsCollected() const {
    return coinsCollected;
}

int Board::getLives() const {
    return lives;
}

void Board::setAt(pair<int, int> check, CellType toChange) {
    grid[check.first][check.second] = toChange;
}

Pacman *Board::getPacmanPtr() const {
    return pacman_ptr.get();
}

const std::vector<std::unique_ptr<Character>> &Board::getGhosts() const {
    return ghosts;
}

int Board::getGridRows() const { return (int)grid.size(); }

int Board::getGridCols() const { return (!grid.empty()) ? (int)grid[0].size() : 0; }

void Board::checkEatenCoins() {
    switch (at(pacman_ptr->getPosition())) {
        case CellType::CoinPoints:
            eatCoinPoints();
            setAt(pacman_ptr->getPosition(), CellType::Empty);
            break;
        case CellType::CoinReverseEating:
            eatCoinReverseEating();
            setAt(pacman_ptr->getPosition(), CellType::Empty);
            break;
        default:
            break;
    }
}

pair<int, int> Board::getFarthestCoordinatesFromPacman() {
    const pair<int, int> directions[] = {{0,  -1},
                                         {0,  1},
                                         {-1, 0},
                                         {1,  0}}; // Up, Down, Left, Right
    vector<vector<bool>> visited(grid.size(), vector<bool>(grid[0].size(), false));
    queue<pair<int, int>> queue;
    pair<int, int> farthestPos = pacman_ptr->getPosition();

    queue.push(farthestPos);
    visited[farthestPos.first][farthestPos.second] = true;

    while (!queue.empty()) {
        pair<int, int> currentPos = queue.front();
        queue.pop();
        farthestPos = currentPos; // update farthest position

        for (const auto &dir: directions) {
            pair<int, int> nextPos = {currentPos.first + dir.first, currentPos.second + dir.second};
            if (isInArea(nextPos) && !visited[nextPos.first][nextPos.second]) {
                CellType cellType = at(nextPos);
                if (cellType != CellType::Wall && cellType != CellType::Teleport) {
                    queue.push(nextPos);
                    visited[nextPos.first][nextPos.second] = true;
                }
            }
        }
    }

    return farthestPos;
}

void Board::addTeleport(pair<int, int> first) {
    // Find the corresponding one -- second, it should be on the other side. if this is x,0 then it must exist on x,size()-1, same for 0,y must exist on size()-1,y
    int rows = (int)grid.size();
    int cols = (int)grid[0].size();

    if ((first.first == 0 && first.second == 0) ||
        (first.first == 0 && first.second == rows - 1) ||
        (first.first == cols - 1 && first.second == 0) ||
        (first.first == cols - 1 && first.second == rows - 1))
    {
        throw std::runtime_error("Teleport nemoze lezat v rohu");
    }

    pair<int, int> second;
    if (first.first == 0) {
        second = make_pair(rows - 1, first.second);
    } else if (first.first == rows - 1) {
        second = make_pair(0, first.second);
    } else if (first.second == 0) {
        second = make_pair(first.first, cols - 1);
    } else if (first.second == cols - 1) {
        second = make_pair(first.first, 0);
    } else {
        throw std::runtime_error("Teleport must be placed on the edge of the grid");
    }

    // Check if the teleport already is counted for
    if (teleports.find(second) != teleports.end() || teleports.find(first) != teleports.end()) {
        return;
    }
    if(at(second) != CellType::Teleport){
        throw std::runtime_error("Teleport must exist on the opposite side");
    }

    teleports.insert(make_pair(first, second));
    teleports.insert(make_pair(second, first));
}


const map<pair<int, int>, pair<int, int>> &Board::getTeleports() const {
    return teleports;
}
