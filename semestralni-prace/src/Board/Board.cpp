//
// Created by stefam on 14. 5. 2023.
//

#include "Board.h"
#include "Configuration.h"


Board::Board(std::vector<std::vector<CellType>> initialGrid, LevelSettings &settings)
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
        for (int x = 0; x < (int)grid.size(); x++) {
            for (int y = 0; y < (int)grid[0].size(); y++) {
                switch (grid[x][y]) {
                    case CellType::CoinPoints:
                    case CellType::CoinReverseEating:
                        coinsToCollect++;
                        break;
                    case CellType::GhostSpawn:
                        createGhost(x, y, ghostsToCreate[ghostSpawns % ghostsToCreate.size()]);
                        grid[x][y] = CellType::Empty;
                        ghostSpawns+=1;
                        break;
                    case CellType::PacmanSpawn:
                        if (pacmanSpawns >= 1)
                            throw std::runtime_error("Na ploche nemoze byt viac ako jeden pacman");
                        grid[x][y] = CellType::Empty;
                        createPacman(x, y);
                        pacmanSpawns+=1;
                        break;
                    case CellType::Teleport:
                        //TODO: TELEPORT
//                        grid[x][y] = CellType::CoinPoints;
//                        if (teleports.find(make_pair(x, y)) != teleports.end()) {
//                            break;
//                        }
//                        if (x != 0 && y != 0 && x != grid.size() - 1 && y != grid.size() - 1) {
//                            throw std::runtime_error("Teleport musi smerovat zo strany na stranu");
//                        }
//                        int nxSur, nySur;
//                        nxSur = grid.size() - 1 - x;
//                        nySur = grid[0].size() - 1 - y;
//                        if (grid[nxSur][nySur] != CellType::Teleport) {
//                            throw std::runtime_error("Teleport musi existovat aj na opacnej strane");
//                        }
//                        teleports.emplace(std::make_pair(x, y), std::make_pair(nxSur, nySur));
//                        teleports.emplace(std::make_pair(nxSur, nySur), std::make_pair(x, y));
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
    checkColissions();
    updateNext = false;

    if (tick % LCM_TICK == 0) {
        updateNext = true;
        vector<pair<int, int>> prevGhostPositions{};
        vector<pair<int, int>> updatedGhostPositions{};
        pair<int, int> prevPacmanPosition = pacman_ptr->getPosition();
        pair<int, int> updatedPacmanPosition;

        for (const auto& ghost : ghosts) {
            prevGhostPositions.push_back(ghost->getPosition());
        }

        movePacman();
        moveGhosts();

        updatedPacmanPosition = pacman_ptr->getPosition(); // Assign the updated pacman position here

        for (const auto& ghost : ghosts) {
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
            moveGhosts();
            updateNext = true;
        } else if (tick % PACMAN_SPEED == 0) {
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
    return (check.first >= 0 && check.first < (int)grid.size() &&
            check.second >= 0 && check.second < (int)grid[0].size());
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
    } else if (type == "RANDOM") {
        std::unique_ptr<IGhostMovement> randomMovement = std::make_unique<RandomMovement>(*this);
        ghosts.emplace_back(std::make_unique<Ghost>(x, y, *this, std::move(randomMovement)));
    } else if (type == "DUMB") {
        std::unique_ptr<IGhostMovement> dumbMovement = std::make_unique<DumbMovement>(*this);
        ghosts.emplace_back(std::make_unique<Ghost>(x, y, *this, std::move(dumbMovement)));
    } else {
        throw std::invalid_argument("Invalid ghost type");
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
    ticksTillReverseEnds = (int)REVERSE_EATING_LENGTH * DEFAULT_TICKS_SECOND;
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

void Board::checkColissions() {
    pair<int, int> pacmanPosition = pacman_ptr->getPosition();
    vector<Character *> ghostsStanding;

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

const map<pair<int, int>, pair<int, int>> &Board::getTeleports() const {
    return teleports;
}

const std::vector<std::vector<CellType>> &Board::getGrid() const { return grid; }

Pacman *Board::getPacmanPtr() const {
    return pacman_ptr.get();
}

const std::vector<std::unique_ptr<Character>> &Board::getGhosts() const {
    return ghosts;
}

int Board::getGridRows() const { return grid.size(); }

int Board::getGridCols() const { return (!grid.empty()) ? grid[0].size() : 0; }
