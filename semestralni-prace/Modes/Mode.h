//
// Created by stefam on 14. 5. 2023.
//
#pragma once


#include <utility>

#include "Renderer.h"
#include "InputHandler.h"

class Game;
class Renderer;

class Mode {
public:
    virtual ~Mode() = default;
    virtual void render(Renderer* renderer, Game* game) = 0;
    virtual void handleInput(InputHandler *inputter, Game * game) = 0;
    bool shouldChange() const {
        return nextChange;
    }
    bool shouldUpdate() const {
        return nextUpdate;
    }
    unique_ptr<Mode>& getNextMode() {
        return nextMode;
    }
protected:
    unique_ptr<Mode> nextMode = nullptr;
    bool nextChange = false;
    bool nextUpdate = false;
};

class MainMenu : public Mode {
public:
    void render(Renderer* renderer, Game* game)  override;
    void handleInput(InputHandler *inputter, Game * game) override;
protected:
    int picked = 0;
};

class Settings : public Mode {
public:
    void render(Renderer* renderer, Game* game)  override;
    void handleInput(InputHandler *inputter, Game * game) override;
};

class RankingTable : public Mode {
public:
    void render(Renderer* renderer, Game* game)  override;
    void handleInput(InputHandler *inputter, Game * game) override;
};

class Playing : public Mode {
public:
    explicit Playing(int difficulty, std::vector<std::vector<CellType>> mapToPlay) : difficulty(difficulty), board(
            make_unique<Board>(std::move(mapToPlay), difficulty)) {
//        board = make_unique<Board>();
    }
    void render(Renderer* renderer, Game* game) override;
    void handleInput(InputHandler *inputter, Game * game) override;

protected:
    int difficulty;
    unique_ptr<Board> board;
};


class ChooseDifficulty : public Mode { ;
public:
    explicit ChooseDifficulty(std::vector<std::vector<CellType>> mapToPlay) : mapToPlay(std::move(mapToPlay)), picked(0) {}
    void render(Renderer* renderer, Game* game) override;
    void handleInput(InputHandler *inputter, Game * game) override;


protected:
    int picked{};
    std::vector<std::vector<CellType>> mapToPlay;
};


class PromptMap : public Mode {
public:
    void render(Renderer* renderer, Game* game) override;
    void handleInput(InputHandler *inputter, Game * game) override;
protected:
    string chosen;
};





