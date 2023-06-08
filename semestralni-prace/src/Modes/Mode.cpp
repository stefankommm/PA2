//
// Created by stefam on 14. 5. 2023.
//


#include "Mode.h"
#include "Renderer.h"
#include "InputHandler.h"
#include "Configuration.h"
#include "GameResultsManager.h"


using namespace std;

Mode::Mode() :
        nextMode(nullptr),
        nextChange(false),
        nextUpdate(false),
        nextEnd(false) {}

bool Mode::shouldChange() const {
    return nextChange;

}

bool Mode::shouldUpdate() const {
    return nextUpdate;
}

unique_ptr<Mode> &Mode::getNextMode() {
    return nextMode;
}

bool Mode::shouldEnd() const {
    return nextEnd;
}

void MainMenu::render() const {
    Renderer::clearScreen();
    Renderer::renderMainMenu(*this);
}

void MainMenu::handleInput() {
    nextUpdate = false;
    Key pressed = InputHandler::getInput();
    if (pressed == Key::Up) {
        nextUpdate = true;
        if (picked == 0)
            picked = 2;
        else
            picked--;
    };
    if (pressed == Key::Down) {
        nextUpdate = true;
        picked++;
        picked %= 3;
    };
    if (pressed == Key::Enter) {
        nextUpdate = true;
        nextChange = true;
        switch (picked) {
            case 0:
                nextMode = make_unique<PromptMap>();
                break;
            case 1:
                nextMode = make_unique<RankingTable>();
                break;
            case 2:
                nextEnd = true;
                break;
        }
    }
}

MainMenu::MainMenu() :
        Mode(),
        picked(0) {}

int MainMenu::getPicked() const {
    return picked;
}


void RankingTable::render() const {
    Renderer::clearScreen();
    Renderer::renderRankingTable(*this, GameResultsManager::getResults());
}

void RankingTable::handleInput() {
    nextUpdate = false;
    Key pressed = InputHandler::getInput();
    if (pressed == Key::Enter || pressed == Key::Esc) {
        nextChange = true;
        nextMode = std::make_unique<MainMenu>();
    }
}

RankingTable::RankingTable() : Mode() {

}


void PromptMap::render() const {
    Renderer::clearScreen();
    InputHandler::disableNonCanonicalMode();
    Renderer::renderPromptMap(*this);
}

void PromptMap::handleInput() {
    nextUpdate = false;
    chosen = InputHandler::getString();
    std::vector<std::vector<CellType>> playing_map;
    try {
        if (chosen.empty()) {
            playing_map = Configuration::loadMapFromFile("map.txt");
        } else {
            playing_map = Configuration::loadMapFromFile(chosen);
        }
    } catch (const std::runtime_error& e) {
        std::cout << rang::style::bold << rang::fg::red << "Chyba: " << rang::style::reset << e.what() << std::endl;
        std::cout << rang::fg::yellow << rang::style::italic << "Zadajte nazov mapy znovu: " << rang::style::reset;
        return;
    }
    
    nextMode = make_unique<ChooseDifficulty>(std::move(playing_map));
    nextChange = true;
}

const string &PromptMap::getChosen() const {
    return chosen;
}

PromptMap::PromptMap() : Mode() {

}


void ChooseDifficulty::render() const {
    std::list<std::string> optionsRender = {"Lahka", "Stredna", "Tazka"};
    Renderer::clearScreen();
    Renderer::renderChooseDifficulty(*this);
}

void ChooseDifficulty::handleInput() {
    nextUpdate = false;
    Key pressed = InputHandler::getInput();
    if (pressed == Key::None) {
        return;
    }
    if (pressed == Key::Up) {
        nextUpdate = true;
        if (picked == 0)
            picked = 2;
        else
            picked--;
    };
    if (pressed == Key::Down) {
        nextUpdate = true;
        picked++;
        picked %= 3;
    };
    if (pressed == Key::Left) {
        nextChange = true;
        nextMode = std::make_unique<MainMenu>();
    }
    if (pressed == Key::Enter) {
        nextChange = true;
        InputHandler::enableNonCanonicalMode();
        LevelSettings settings = Configuration::loadConfiguration(picked);
        nextMode = std::make_unique<Playing>(std::move(mapToPlay), settings, picked);
    }
}

ChooseDifficulty::ChooseDifficulty(std::vector<std::vector<CellType>> mapToPlay )
        : Mode(),
          picked(0),
          mapToPlay(std::move(mapToPlay))
{}

int ChooseDifficulty::getPicked() const {
    return picked;
}

const list<std::string> &ChooseDifficulty::getOptions() const {
    return options;
}

void PromptName::render() const {
    Renderer::clearScreen();
    Renderer::renderPromptName(*this);
}

void PromptName::handleInput() {
    nextUpdate = false;
    InputHandler::disableNonCanonicalMode();
    name = InputHandler::getString();
    InputHandler::enableNonCanonicalMode();

    if (!name.empty()) {
        GameResultsManager::addResult({name, score});
    }
    nextMode = make_unique<RankingTable>();
    nextChange = true;
}

int PromptName::getScore() const {
    return score;
}

const string &PromptName::getName() const {
    return name;
}





