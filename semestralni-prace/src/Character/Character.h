//
// Created by stefam on 14. 5. 2023.
//
#pragma once

#include <memory>
#include <iomanip>
#include <utility>
#include <vector>
#include <string>
#include <map>

#include "Board/Board.h"

class Board;
class IGhostMovement;

using namespace std;

/**
 * @class Character
 * @brief Represents a character in the game.
 *
 * The Character class serves as a base class for Pacman and Ghost characters.
 */
class Character {
public:
    Character(int x, int y, Board &board) :
            board(board),
            xSur(x),
            ySur(y),
            spawnPoint({x, y}),
            alive(true),
            reverseEating(false)
            {};
    virtual ~Character() = default;
    virtual void move() = 0;
    [[nodiscard]] virtual char getSymbol() const = 0;
    virtual void setReverseEating(bool state) = 0;
    /**
     * @brief Checks if the character is alive.
     * @return True if the character is alive, false otherwise.
     */
    [[nodiscard]] bool isAlive() const;

    /**
     * @brief Checks if the character is in reverse eating mode.
     * @return True if the character is in reverse eating mode, false otherwise.
     */
    [[nodiscard]] bool isReverseEating() const;
    /**
     * @brief Respawns the character at its spawn point.
     */
    virtual void respawn();
    /**
     * @brief Retrieves the position of the character.
     * @return The position of the character as a pair of integers (x, y).
     */
    [[nodiscard]] pair<int, int> getPosition() const;
    /**
     * @brief Sets the position of the character.
     * @param newPos The new position of the character as a pair of integers (x, y).
     */
    void setPosition(pair<int,int>);
    static pair<int, int> getNextPosition(const pair<int, int> &position, Direction direction) {
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


protected:
    Board &board;
    int xSur{}, ySur{};
    pair<int, int> spawnPoint;
    bool alive;
    bool reverseEating;
};

/**
 * @class Pacman
 * @brief Represents the Pacman character in the game.
 *
 * The Pacman class inherits from the Character class and adds Pacman-specific functionality.
 */
class Pacman : public Character {
public:
    /**
     * @brief Constructor for the Pacman class.
     * @param x The X-coordinate of Pacman's initial position.
     * @param y The Y-coordinate of Pacman's initial position.
     * @param board The game board on which Pacman is located.
     */
    Pacman(int x, int y, Board &board);
    /**
     * @brief Retrieves the symbol representing Pacman's current direction.
     * @return The symbol representing Pacman's current direction.
     */
    [[nodiscard]] char getSymbol() const override;
    /**
     * @brief Moves Pacman according to its current direction.
     */
    void move() override;
    /**
     * @brief Sets the direction of Pacman's movement.
     * @param dir The direction to set for Pacman.
     */
    void setDirection(Direction dir);
    /**
     * @brief Retrieves the direction of Pacman's movement.
     * @return The direction of Pacman's movement.
     */
    [[nodiscard]] Direction getDirection() const;
    /**
     * @brief Sets the reverse eating mode for Pacman.
     * @param state The state to set for the reverse eating mode.
     */
    virtual void setReverseEating(bool state) override;


private:
    Direction direction;
    std::map<Direction, char> directionSymbols{
            {Direction::Up,    '^'},
            {Direction::Down,  'v'},
            {Direction::Left,  '<'},
            {Direction::Right, '>'}
    };
};

/**
 * @class Ghost
 * @brief Represents a ghost character in the game.
 *
 * The Ghost class inherits from the Character class and adds ghost-specific functionality.
 */
class Ghost : public Character {
public:
    /**
     * @brief Constructor for the Pacman class.
     * @param x The X-coordinate of Pacman's initial position.
     * @param y The Y-coordinate of Pacman's initial position.
     * @param board The game board on which Pacman is located.
     */
    Ghost(int x, int y, Board &board, unique_ptr<IGhostMovement> movementStrategy)
            : Character(x, y, board),
            isAtHome(true),
            movementStrategy(std::move(movementStrategy))
             {}

    /**
     * @brief Retrieves the symbol representing the ghost.
     * @return The symbol representing the ghost.
     */
    [[nodiscard]] char getSymbol() const override;
    /**
     * @brief Moves the ghost according to its movement strategy.
     */
    void move() override;
    /**
     * @brief Respawns the ghost at its spawn point.
     */
    void respawn() override;

    /**
     * @brief Sets the state of whether the ghost is at its home.
     * @param state The state to set for whether the ghost is at its home.
     */
    void setIsAtHome(bool state);
    /**
     * @brief Sets the reverse eating mode for the ghost.
     * @param state The state to set for the reverse eating mode.
     */
    void setReverseEating(bool state) override;

private:
    bool isAtHome;
    std::unique_ptr<IGhostMovement> movementStrategy;
};


