//
// Created by stefam on 14. 5. 2023.
//

#pragma once

#include <utility>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <cmath>
#include "libraries/rang.hpp"

#include "constants.h"
#include "Character/Character.h"
#include "Character/Pacman.h"
#include "IGhostMovement.h"

class Board;
class Character;
class Pacman;
class Ghost;
class SmartMovement;

using namespace std;

/**
 * @brief Represents the game board for a Pacman-like game.
 */
class Board {
public:
    /**
     * @brief Constructor for Board class.
     * @param initialGrid The initial grid configuration of the game board.
     * @param settings The level settings for the game.
     */
    Board(std::vector<std::vector<CellType>> initialGrid, LevelSettings & settings);
    ~Board() = default;

    /**
     * @brief Creates a Ghost at the specified coordinates with the given type.
     *
     * This function creates a Ghost at the given (x, y) coordinates and adds it to the ghosts array.
     * An error is thrown if the provided type is unknown.
     *
     * @param x The X-Coordinate of the point where the GHOST should be generated.
     * @param y The Y-Coordinate of the point where the GHOST should be generated.
     * @param type The type of the ghost to be added to the ghosts array.
     */
    void createGhost(int x, int y, const string & type);

    /**
     * @brief Creates a Pacman at the specified coordinates.
     *
     * @param x X-Coordinate of the point where the PACMAN should be generated.
     * @param y Y-Coordinate of the point where the PACMAN should be generated.
     */
    void createPacman(int x, int y);

    /**
     * @brief Initializes the game board with the given ghost configurations.
     *
     * @param ghostsToCreate Array of strings corresponding to the types of the Ghosts to be created.
     */
    void initialize(const std::vector<std::string> & ghostsToCreate);

    /**
     * @brief Checks if the map should be updated on the next tick.
     *
     * @return True if the map should be updated, false otherwise.
     */
    bool shouldUpdate() const;

    /**
     * @brief Updates the positions of Pacman and Ghosts according to their configured speed.
     *        Checks if Pacman collides with any Ghosts.
     *
     * @param tick The current game tick.
     */
    void updatePositions(unsigned long long tick);

    /**
     * @brief Checks if Pacman's position is the same as any of the Ghosts' positions.
     *        Performs corresponding actions for the Ghost and Pacman.
     */
    void checkColissions();

    /**
     * @brief Moves all Ghosts by calling their corresponding IGhostMovement.move() function.
     */
    void moveGhosts();

    /**
     * @brief Moves Pacman by calling Pacman.move() function.
     */
    void movePacman();

    /**
     * @brief Rotates Pacman according to the user input.
     *
     * @param dir The direction to rotate Pacman.
     */
    void rotatePacman(Direction dir);

    /**
     * @brief Retrieves the CellType at the specified coordinates in the grid.
     *
     * @param check The position to check in the grid.
     * @return The CellType of the grid cell.
     */
    CellType at(pair<int,int> check) const;

    /**
     * @brief Checks if the current state of the game is final.
     *
     * @return True if the player lost all lives or collected all coins, false otherwise.
     */
    bool isEnd() const;

    /**
     * @brief Retrieves whether reverse eating is enabled.
     *
     * @return True if reverse eating is enabled, false otherwise.
     */
    bool isReverseEating() const;

    /**
     * @brief Retrieves the count of Ghosts eaten during reverse eating.
     *
     * @return The count of Ghosts eaten during reverse eating.
     */
    int getReverseEatingEated() const;

    /**
     * @brief Retrieves the count of coins that need to be collected.
     *
     * @return The count of coins that need to be collected.
     */
    int getCoinsToCollect() const;

    /**
     * @brief Retrieves the count of coins already collected.
     *
     * @return The count of coins already collected.
     */
    int getCoinsCollected() const;

    /**
     * @brief Retrieves the remaining number of lives.
     *
     * @return The remaining number of lives.
     */
    int getLives() const;

    /**
     * @brief Retrieves the current score.
     *
     * @return The current score.
     */
    int getScore() const;

    /**
     * @brief Retrieves the current position of Pacman.
     *
     * @return The current position of Pacman.
     */
    pair<int,int> getPacmanPosition() const;

    /**
     * @brief Retrieves the (x,y) coordinates of the border that each Ghost has to cross before leaving spawn.
     *
     * @return The (x,y) coordinates of the border.
     */
    pair<int,int> getBorderToCross() const;

    /**
     * @brief Checks if the specified coordinates correspond to a Ghost's position.
     *
     * @param xSur The X-coordinate to check.
     * @param ySur The Y-coordinate to check.
     * @return The index of the Ghost if found, -1 otherwise.
     */
    int isGhostPosition(int xSur, int ySur) const;

    /**
     * @brief Retrieves the row count of the grid.
     *
     * @return The row count of the grid.
     */
    int getGridRows() const;

    /**
     * @brief Retrieves the column count of the grid.
     *
     * @return The column count of the grid.
     */
    int getGridCols() const;

    /**
     * @brief Checks if the specified coordinates are within the accessible area of the grid.
     *
     * @param check The coordinates to check.
     * @return True if the coordinates are within the accessible area, false otherwise.
     */
    bool isInArea(pair<int,int> check) const;

    /**
     * @brief Retrieves the number of ticks remaining until the reverse eating effect ends.
     *
     * @return The number of ticks remaining until the reverse eating effect ends.
     */
    int getTicksTillReverseEnds() const;

    /**
     * @brief Retrieves the reference to the grid.
     *
     * @return The reference to the grid.
     */
    const std::vector<std::vector<CellType>>& getGrid() const;

    /**
     * @brief Retrieves the pointer to the Pacman character.
     *
     * @return The pointer to the Pacman character.
     */
    Pacman* getPacmanPtr() const;

    /**
     * @brief Retrieves the reference to the vector of Ghost characters.
     *
     * @return The reference to the vector of Ghost characters.
     */
    const std::vector<std::unique_ptr<Character>>& getGhosts() const;

    /**
     * @brief Sets the CellType at the specified coordinates in the grid.
     *
     * @param check The coordinates to change.
     * @param toChange The CellType to set at the specified coordinates.
     */
    void setAt(pair<int,int> check, CellType toChange);

    /**
     * @brief Sets the number of ticks remaining until the reverse eating effect ends.
     *
     * @param ticksTillReverseEnds The new value to set for the number of ticks.
     */
    void setTicksTillReverseEnds(int ticksTillReverseEnds);

    /**
     * @brief Respawns Pacman by calling the corresponding respawn() function.
     */
    void respawnPacman();

    /**
     * @brief Respawns all Ghosts by calling their corresponding respawn() functions.
     */
    void respawnGhosts();

    /**
     * @brief Respawns all Ghosts and Pacman.
     */
    void respawnAll();

    /**
     * @brief Performs actions when Pacman eats a CoinPoints coin.
     */
    void eatCoinPoints();

    /**
     * @brief Performs actions when Pacman eats a CoinReverseEating coin.
     */
    void eatCoinReverseEating();

    /**
     * @brief Enables reverse eating for all Ghosts and sets the duration.
     */
    void enableReverseEating();

    /**
     * @brief Disables reverse eating.
     */
    void disableReverseEating();

    /**
     * @brief Performs actions when Pacman is consumed by a Ghost.
     */
    void pacmanEaten();

    /**
     * @brief Performs actions when a Ghost is consumed by Pacman during reverse eating.
     *
     * @param ghost The Ghost character that is consumed by Pacman.
     */
    void reverseGhostEaten(Character &ghost);

    /**
     * @brief Calculates the least common multiple (LCM) of two numbers.
     *
     * @param a The first number.
     * @param b The second number.
     * @return The LCM of the two numbers.
     */
    static int calculateLCM(int a, int b);

    /**
     * @brief Retrieves the map of teleport coordinates and their corresponding destinations.
     *
     * @return The map of teleport coordinates and their corresponding destinations.
     */
    const map<pair<int, int>, pair<int, int>> &getTeleports() const;

private:
    std::vector<std::vector<CellType>> grid;

    int ticksTillReverseEnds;
    bool updateNext;

    bool reverseEating;
    int reverseEatingEated;

    int coinsToCollect = 0;
    int coinsCollected = 0;
    int score = 0;
    int lives = 3;

    pair<int,int> borderToCross;
    unique_ptr<Pacman> pacman_ptr;
    std::vector<std::unique_ptr<Character>> ghosts;
    map<pair<int,int>,pair<int,int>> teleports;

    int REVERSE_EATING_LENGTH;
    int PACMAN_SPEED;
    int GHOST_SPEED;
    int LCM_TICK;
};
