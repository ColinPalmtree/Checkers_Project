#include <iostream>
#include "Game.h"

int main() {
    try {
        Game game;
        // starts the GUI/opens the window
        game.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}