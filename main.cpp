#include "src/game.hpp"

int main()
{
    // create game instance
    Game game {};

    return game.run();  // if an error occure, Game::run() will return 1, else 0 (EXIT_SUCCESS)
}
