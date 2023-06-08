
#include "Renderer.h"


using namespace std;

void Renderer::clearScreen() {
    system("clear");
}


void Renderer::renderBoard(const Board &board) {
    const auto &pacman_ptr = board.getPacmanPtr();
    const auto &ghosts = board.getGhosts();


    for (int y = 0; y < board.getGridCols(); y++) {
        for (int x = 0; x < board.getGridRows(); x++) {
            if (pacman_ptr->getPosition() == make_pair(x, y)) {
                if (board.at({x, y}) == CellType::Teleport) {
                    std::cout << rang::bg::blue << rang::fgB::yellow << pacman_ptr->getSymbol() << rang::style::reset;
                } else {
                    std::cout << rang::fgB::yellow << pacman_ptr->getSymbol() << rang::style::reset;
                }
                continue;
            }
            int isAGhost = board.isGhostPosition(x, y);
            if (isAGhost >= 0) {
                if (board.at({x, y}) == CellType::Border) {
                    if (ghosts[isAGhost]->isReverseEating()) {
                        std::cout << rang::bg::gray << rang::style::bold << rang::fgB::green
                                  << ghosts[isAGhost]->getSymbol() << rang::style::reset;
                    } else {
                        std::cout << rang::bg::gray << rang::style::bold << rang::fgB::red
                                  << ghosts[isAGhost]->getSymbol() << rang::style::reset;
                    }
                } else {
                    if (ghosts[isAGhost]->isReverseEating()) {
                        std::cout << rang::fgB::green << ghosts[isAGhost]->getSymbol() << rang::style::reset;
                    } else {
                        std::cout << rang::fgB::red << ghosts[isAGhost]->getSymbol() << rang::style::reset;
                    }
                }
            } else {
                switch (board.at({x, y})) {
                    case CellType::Spawn:
                    case CellType::Empty:
                        std::cout << " ";
                        break;
                    case CellType::Wall:
                        std::cout << rang::bgB::blue << " " << rang::style::reset;
                        break;
                    case CellType::CoinPoints:
                        std::cout << "·";
                        break;
                    case CellType::Pacman:
                        std::cout << rang::fgB::yellow << pacman_ptr->getSymbol() << rang::style::reset;
                        break;
                    case CellType::CoinReverseEating:
                        std::cout << rang::fgB::magenta << "C" << rang::style::reset;
                        break;
                    case CellType::Border:
                        std::cout << rang::bg::gray << rang::style::bold << " " << rang::style::reset;
                        break;
                    case CellType::Ghost:
                        std::cout << rang::fgB::red << "G" << rang::style::reset;
                        break;
                    case CellType::PacmanSpawn:
                        std::cout << rang::fgB::yellow << "▸" << rang::style::reset;
                        break;
                    case CellType::Teleport:
                        std::cout << rang::bg::blue << " " << rang::style::reset;
                        break;
                    default:
                        break;
                }
            }
        }
        std::cout << std::endl;
    }
}


void Renderer::renderBoardStats(const Board &board) {

    if (board.isReverseEating()) {
        std::cout << rang::fgB::green << rang::style::bold << "Do konca reverse eating je " << rang::style::reset;
        std::cout << ((double) board.getTicksTillReverseEnds()) / 20 << endl;

        std::cout << rang::fgB::green << rang::style::bold << "Odmena za dalsieho ducha: " << rang::style::reset;
        std::cout << (int) std::pow(2, board.getReverseEatingEated() + 1) * 100 << endl;
    }

    std::cout << rang::fgB::yellow << rang::style::bold << "Score: " << rang::style::reset;
    std::cout << board.getScore() << endl;

    std::cout << rang::fgB::red << rang::style::bold << "Lives: " << rang::style::reset;
    std::cout << board.getLives() << endl;

    std::cout << rang::fgB::magenta << rang::style::bold << "Coins left to collect: " << rang::style::reset;
    std::cout << (board.getCoinsToCollect() - board.getCoinsCollected()) << endl;
}


void Renderer::renderMainMenu(const MainMenu &menu) {
    for (int i = 0; i < 3; i++) {
        if (i != menu.getPicked()) {
            std::cout << menu.options[i] << endl;
        } else {
            std::cout << rang::style::bold << menu.options[i] << rang::style::reset << std::endl;
        }
    }
}

void Renderer::renderRankingTable(const RankingTable &table, const std::vector<std::pair<std::string, int>> & results) {
    std::cout << rang::fg::yellow << rang::style::italic << "Ranking Table of the best players (Showing 10 first):"
              << rang::style::reset << endl;
    int poradie = 1;
    for(const auto & i : results){
        if(poradie > 10){
            cout << endl;
            break;
        }
        cout << poradie++ << ". " <<  i.first << " has score " << i.second << endl;
    }

}

void Renderer::renderPromptMap(const PromptMap &map) {
    std::cout << rang::fg::yellow << rang::style::italic << "Zadajte nazov mapy(map.txt -> Stlacte enter): "
              << rang::style::reset << map.getChosen();
}

void Renderer::renderChooseDifficulty(const ChooseDifficulty &map) {
    int index = 0;
    for (const auto &option: map.getOptions()) {
        if (index != map.getPicked()) {
            std::cout << option << std::endl;
        } else {
            std::cout << rang::style::bold << option << rang::style::reset << std::endl;
        }
        index++;
    }
}

void Renderer::renderPromptName(const class PromptName &prompt) {
    std::cout << rang::fg::yellow << rang::style::italic << "Mate skore: " << rang::style::reset << prompt.getScore()
              << endl;
    std::cout << rang::fg::yellow << rang::style::italic << "Zadajte nazov mena: " << rang::style::reset
              << prompt.getName();

}


