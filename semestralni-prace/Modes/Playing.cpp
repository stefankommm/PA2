#include "Mode.h"
#include <iostream>
using namespace std;

void Playing::handleInput(InputHandler *inputter, Game *game) {
    Key pressed = inputter->getInput();
    if (pressed == Key::Enter) {
        nextChange = true;
        nextMode = std::make_unique<MainMenu>();
    }
    switch (pressed) {
        case Key::Up:
            board->rotatePacman(Direction::Up);
            break;
        case Key::Down:
            board->rotatePacman(Direction::Down);
            break;
        case Key::Right:
            board->rotatePacman(Direction::Right);
            break;
        case Key::Left:
            board->rotatePacman(Direction::Left);
            break;
    }

}

void Playing::render(Renderer* renderer, Game *game) {
    renderer->clearScreen();
//    renderer->renderPlaying(this);
    cout << "Playing" << difficulty << endl;
    cerr << "RENDERING" << endl;
    board->updatePositions(tick);
    board->render();
    nextUpdate = true;

}

void Playing::tickAction() {
    tick++;
}
