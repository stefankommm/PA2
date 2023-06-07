//
// Created by stefam on 14. 5. 2023.
//
#pragma once

#include <utility>
#include <list>
#include <memory>
#include <string>
#include "constants.h"
#include "Board/Board.h"


using namespace std;

class Game;
class Renderer;

class Mode {
public:
    explicit Mode();
    virtual ~Mode() = default;
    virtual void render() const = 0;
    virtual void handleInput() = 0;
    virtual void tickAction() { };
    [[nodiscard]] bool shouldChange() const;
    bool shouldUpdate() const;
    bool shouldEnd() const;
    unique_ptr<Mode> &getNextMode();

protected:
    unique_ptr<Mode> nextMode;
    bool nextChange;
    bool nextUpdate;
    bool nextEnd;
};

class MainMenu : public Mode {
public:
    explicit MainMenu();
    void render() const override;
    void handleInput() override;
    int getPicked() const;

    const std::string options[4] = {"Hrat", "Nastavenia", "Ranking Table", "Ukoncit"};
protected:
    int picked;
};

class Settings : public Mode {
public:
    explicit Settings();
    void render() const override;
    void handleInput() override;
};

class RankingTable : public Mode {
public:
    explicit RankingTable();
    void render() const override;
    void handleInput() override;
};

class Playing : public Mode {
public:
    explicit Playing(std::vector<std::vector<CellType>> mapToPlay, LevelSettings settings, int difficulty);
    ~Playing() = default;
    void render() const override;
    void handleInput() override;
    void tickAction() override;

protected:
    int difficulty;
    unsigned long long tick;
    unique_ptr<Board> board;
};


class ChooseDifficulty : public Mode {
public:
    explicit ChooseDifficulty(std::vector<std::vector<CellType>> mapToPlay);
    void render() const override;
    void handleInput() override;
    [[nodiscard]] int getPicked() const;
    [[nodiscard]] const list<std::string> &getOptions() const;

protected:
    int picked;
    std::list<std::string> options = {"Lahka", "Stredna", "Tazka"};
    std::vector<std::vector<CellType>> mapToPlay;
};


class PromptMap : public Mode {
public:
    explicit PromptMap();
    void render() const override;
    void handleInput() override;
    [[nodiscard]] const string &getChosen() const;

private:
    string chosen;
};


class PromptName : public Mode {
public:
    explicit PromptName(int score, int difficutly) : Mode(), difficulty(difficutly),     score(score) {}
    void render() const override;
    void handleInput() override;
    [[nodiscard]] int getScore() const;
    [[nodiscard]] const string &getName() const;

protected:
    int difficulty;
    int score;
    string name;
};




