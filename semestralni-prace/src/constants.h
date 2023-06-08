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

/**
 * @brief CellType is representing the cells on the grid.
 * Enum for the types the grid can hold.
 */
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
/**
 * @brief Predefined struct for the loading of the settings
 */
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

