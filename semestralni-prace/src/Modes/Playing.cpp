#include "Mode.h"

using namespace std;

void Playing::handleInput( ) {
    nextUpdate = true;
    Key pressed = InputHandler::getInput();
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
        default:
            break;
    }
}

void Playing::render() const {
    if(board->shouldUpdate()){
        Renderer::clearScreen();
        Renderer::renderBoard(*board);
        Renderer::renderBoardStats(*board);
    }
}

void Playing::tickAction() {
    tick++;
    board->updatePositions(tick);
    if(board->isReverseEating()){
        if(board->getTicksTillReverseEnds() > 0)
            board->setTicksTillReverseEnds(board->getTicksTillReverseEnds()-1);
        if(board->getTicksTillReverseEnds() == 0){
            board->disableReverseEating();
        }
    }
    if(board->isEnd()){
        int score = board->getScore();
        nextMode = std::make_unique<PromptName>(score);
        nextChange = true;
    }
}

Playing::Playing(std::vector<std::vector<CellType>> mapToPlay ,LevelSettings settings, int difficulty)
    : Mode(),
      tick(0),
      board(make_unique<Board>(std::move(mapToPlay), settings))
    {}
