//
// Created by stefam on 14. 5. 2023.
//

#include "Mode.h"
#include "rang.hpp"
#include <list>
#include "Configuration.h"

using namespace std;



void MainMenu::render(Renderer* renderer, Game *game) {
    nextUpdate = false;
    std::string options[4] = {"Hrat", "Nastavenia", "Ranking Table", "Ukoncit"};
    renderer->clearScreen();
    for(int i = 0 ; i < 4 ; i++){
        if(i!=picked){
            std::cout << options[i] << endl;
        } else {
            std:: cout << rang::style::bold << options[i] << rang::style::reset << std::endl;
        }
    }
}

void MainMenu::handleInput(InputHandler *inputter, Game *game) {
    Key pressed = inputter->getInput();
    nextUpdate = false;
    if(pressed == Key::Up){
        nextUpdate = true;
        cout << "UP Pressed" << endl;
        if(picked == 0)
            picked = 2;
        else
            picked--;
    };
    if(pressed == Key::Down){
        nextUpdate = true;
        cout << "Down Pressed" << endl;
        picked++;
        picked %=4;
    };
    if(pressed == Key::Enter){
        nextUpdate = true;
        nextChange = true;
        switch(picked){
            case 0:
                nextMode = make_unique<PromptMap>();
                break;
            case 1:
                nextMode = make_unique<Settings>();
                break;
            case 2:
                nextMode = make_unique<RankingTable>();
                break;
            case 3:
                game->requestEnd();
                break;
        }
    }
}


void Settings::render(Renderer* renderer, Game *game) {
    renderer->clearScreen();
    cout << "Settings" << endl;
    nextUpdate = false;
}

void Settings::handleInput(InputHandler *inputter, Game *game) {
    Key pressed = inputter->getInput();
    if (pressed == Key::Enter) {
        nextChange = true;
        nextMode = std::make_unique<MainMenu>();
    }
}

void RankingTable::render(Renderer* renderer, Game *game) {
    renderer->clearScreen();
    cout << "Ranking Table:" << endl;
    for(auto & i : game->getResults()){
        cout << i.first << "     " << i.second << endl;
    }
    nextUpdate = false;
}

void RankingTable::handleInput(InputHandler *inputter, Game *game) {
    Key pressed = InputHandler::getInput();
    if (pressed == Key::Enter) {
        nextChange = true;
        nextMode = std::make_unique<MainMenu>();
    }
}


void PromptMap::render(Renderer* renderer, Game *game) {
    nextUpdate = false;
    renderer->clearScreen();
    std::cout <<  rang::fg::yellow << rang::style::italic << "Zadajte nazov mapy: " << rang::style::reset << chosen;
}

void PromptMap::handleInput(InputHandler *inputter, Game *game) {
    chosen = inputter->getString();
    std::vector<std::vector<CellType>> playing_map;
    try {
        playing_map = Configuration::loadMapFromFile(chosen);

    } catch (const std::runtime_error& e) {
        std::cout << rang::style::bold	<<  rang::fg::red << "Chyba: " << rang::style::reset << e.what() << std::endl;
        std::cout <<  rang::fg::yellow << rang::style::italic << "Zadajte nazov mapy znovu: " << rang::style::reset;
        return;
    }
    nextMode = make_unique<ChooseDifficulty>(std::move(playing_map));
    nextChange = true;

}



void ChooseDifficulty::render(Renderer *renderer, Game *game) {
    nextUpdate = false;
    std::list<std::string> options = {"Lahka", "Stredna", "Tazka"};
    renderer->clearScreen();
    int index = 0;
    for (const auto& option : options) {
        if (index != picked) {
            std::cout << option << std::endl;
        } else {
            std::cout << rang::style::bold << option << rang::style::reset << std::endl;
        }
        index++;
    }
}

void ChooseDifficulty::handleInput(InputHandler *inputter, Game *game) {
    Key pressed = InputHandler::getInput();
    if(pressed == Key::None){
        return;
    }
    if(pressed == Key::Up){
        nextUpdate = true;
        if(picked == 0)
            picked = 2;
        else
            picked--;
    };
    if(pressed == Key::Down){
        nextUpdate = true;
        picked++;
        picked %=3;
    };
    if (pressed == Key::Left) {
        nextChange = true;
        nextMode = std::make_unique<MainMenu>();
    }
    if(pressed == Key::Enter) {
        nextChange = true;
        InputHandler::enableNonCanonicalMode();
        nextMode = std::make_unique<Playing>(picked, std::move(mapToPlay));
    }
}
