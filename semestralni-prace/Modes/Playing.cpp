#include "Mode.h"
#include <iostream>
using namespace std;

void Playing::handleInput(InputHandler *inputter, Game *game) {
    Key pressed = inputter->getInput();
    if (pressed == Key::Enter) {
        nextChange = true;
        nextMode = std::make_unique<MainMenu>();
    }
}

void Playing::render(Renderer* renderer, Game *game) {
    renderer->clearScreen();
//    renderer->renderPlaying(this);
    cout << "Playing" << difficulty << endl;
    board->render();
    nextUpdate = false;
}
