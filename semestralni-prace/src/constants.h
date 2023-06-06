#pragma once
#include <vector>

enum class Direction {
    Up,
    Down,
    Left,
    Right
};
const std::vector<Direction> AllDirections = {
        Direction::Up,
        Direction::Down,
        Direction::Left,
        Direction::Right
};

enum class CellType {
    Empty,
    Wall,
    Border,
    CoinPoints,
    CoinReverseEating,
    GhostSpawn,
    PacmanSpawn,
    Teleport,
    Pacman,
    Ghost,
    Spawn
};

struct LevelSettings {
    std::vector<std::string> ghosts;
    int ghostSpeed;
    int pacmanSpeed;
    double bonusProbability;
    int reverseEatingLength;
};

const int DEFAULT_PACMAN_SPEED = 2;
const int DEFAULT_GHOST_SPEED = 2;
const int DEFAULT_TICKS_SECOND = 20;
const int DEFAULT_COINPOINTS_SCORE = 20;

enum class GhostType {
    SmartGhost,
    DumbGhost,
    RandomGhost
};