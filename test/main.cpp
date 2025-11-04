#include "Game.h"

int main(void) {

    Game &game = Game::getInstance();
    game.init();
    game.run();

    return 0;
}