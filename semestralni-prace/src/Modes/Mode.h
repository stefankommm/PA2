//
// Created by stefam on 14. 5. 2023.
//
#pragma once

#include <utility>
#include <list>
#include <memory>
#include <string>
#include "Board/Board.h"


using namespace std;

class Game;
class Renderer;

/**
 * @brief The base class for all game modes.
 * Mode is the class from which all the application modes are derived from.
 */
class Mode {
public:
    explicit Mode();
    virtual ~Mode() = default;
    virtual void render() const = 0;
    virtual void handleInput() = 0;
    virtual void tickAction() { };
    /**
    * @brief Checks if the game mode should change to the next mode.
    * @return True if the mode should change.
    */
    [[nodiscard]] bool shouldChange() const;
    /**
    * @brief Checks if the game mode should update.
    * @return True if the mode should update.
    */
    [[nodiscard]] bool shouldUpdate() const;
    /**
    * @brief Checks if the game mode should end.
    * @return True if the mode should end.
    */
    [[nodiscard]] bool shouldEnd() const;
    /**
    * @brief Retrieves the next game mode.
    * @return Reference to the next game mode.
    */
    unique_ptr<Mode> &getNextMode();
protected:
    unique_ptr<Mode> nextMode;
    bool nextChange;
    bool nextUpdate;
    bool nextEnd;
};

/**
 * @brief MainMenu serves the purpose of handling the menu which is displayed when the game starts
 * MainMenu allows user to choose one of the item in the menu and change mode to the corresponding one.
 */
class MainMenu : public Mode {
public:
    explicit MainMenu();
    /**
     * @brief Renders the MainMenu by calling the Renderer::renderMainMenu()
     */
    void render() const override;
    /**
     * Handles user input
     */
    void handleInput() override;
    [[nodiscard]] int getPicked() const;

    const std::string options[3] = {"Hrat", "Ranking Table", "Ukoncit"};
protected:
    int picked;
};

/**
 * @brief Mode which displays user the best players, that have played game during the game is open.
 */
class RankingTable : public Mode {
public:
    explicit RankingTable();
    /**
     * Renders the RankingTable by calling the appropriate Render function
     */
    void render() const override;
    /**
     * Handles the input if the mode should be switched to MainMenu
     */
    void handleInput() override;
};

/**
 * @brief Mode corresponding to the main part of the game which is playing
 * Playing is in charge of rendering, getting input for the game.
 */
class Playing : public Mode {
public:
    explicit Playing(std::vector<std::vector<CellType>> mapToPlay ,const LevelSettings& settings, int difficulty);
    ~Playing() = default;
    /**
     * @brief Renders the latest information about the game on the screen.
     */
    void render() const override;
    /**
     * @brief Handles user input while playing. Rotates the pacman appropriately to the KEYS
     */
    void handleInput() override;
    /**
     * @brief Action which should be executed on every tick. Tells the Board to update the position of the Characters.
     * Function which handles updating the position of the Characters on the board, decrements the ticksTillReverseEnds
     * when the reverseEating is activated. Asks the board if the state of the game is finished, in which case sets
     * the nextEnd, nextChange on true and corresponding nextMode to the PromptName
     */
    void tickAction() override;

protected:
    vector<LevelSettings> difficulties;
    int difficulty;
    unsigned long long tick;
    unique_ptr<Board> board;
};

/**
 * @brief Mode which handles the RadioBox of the difficulties that can be played by the user
 */
class ChooseDifficulty : public Mode {
public:
    explicit ChooseDifficulty(std::vector<std::vector<CellType>> mapToPlay);
    /**
     * Handles rendering of the Choose difficulty by calling appropriate static method in Renderer
     */
    void render() const override;
    /**
     * @brief Handles switching the picked user choice by UP/DOWN keys, accepts the mode on pressing ENTER
     * If the user has selected the difficulty the Playing mode is set as nextMode
     */
    void handleInput() override;
   /**
    * Getter for picked
    * @brief Getter for picked
    * @return selected user difficulty.
    */
    [[nodiscard]] int getPicked() const;
    /**
     * Returns the vector of options to pick
     * @return the reference to the vector of the strings of the difficulty
     */
    [[nodiscard]] const list<std::string> &getOptions() const;

protected:
    int picked;
    std::list<std::string> options = {"Lahka", "Stredna", "Tazka"};
    std::vector<std::vector<CellType>> mapToPlay;
};

/**
 * @brief Mode which allows user to enter the filename to the map which should be played.
 * Handles user selection of the map to play.
 */
class PromptMap : public Mode {
public:
    explicit PromptMap();
    /**
     * @Renders text to prompt the user to input filename.
     */
    void render() const override;
    /**
     * Handles the inputted filename. Calls the loadMap function and if everything went fine sets the nextMode to the ChooseDifficulty
     */
    void handleInput() override;
    [[nodiscard]] const string &getChosen() const;

private:
    string chosen;
};

/**
 * @brief Mode displayed after ending the game prompting user to enter his/her name for the ranking table.
 */
class PromptName : public Mode {
public:
    explicit PromptName(int score, int difficutly) : Mode(), difficulty(difficutly), score(score) {}
    /**
     * Renders the prompt to enter the user input
     */
    void render() const override;
    /**
     * If the name is written the handleInput sets the nextMode to RankingTable
     */
    void handleInput() override;
    /**
     * @brief Getter for the score
     * @return the score user has achieved in the game.
     */
    [[nodiscard]] int getScore() const;
    /**
     * @brief Getter for the name
     * @return the name user has already written.
     */
    [[nodiscard]] const string &getName() const;

protected:
    int difficulty;
    int score;
    string name;
};




