#pragma once
#include <vector>
#include <chrono>

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

enum class Difficulty {
    Easy,
    Medium,
    Hard
};
//
//std::map<int, Difficulty> DifficultyDictionary = {
//        {0, Difficulty::Easy},
//        {1, Difficulty::Medium},
//        {2, Difficulty::Hard}
//};


struct LevelSettings {
    std::vector<std::string> ghosts;
    int ghostSpeed;
    int pacmanSpeed;
    int reverseEatingLength;
};

const std::chrono::milliseconds DEFAULT_TICK(50); // 1000ms / 20 = 50ms // 20FPS
const int DEFAULT_TICKS_SECOND = 20;
const int DEFAULT_LIVES = 3;
const char DEFAULT_GHOST_SYMBOL = 'G';



enum class GhostType {
    SmartGhost,
    DumbGhost,
    RandomGhost
};