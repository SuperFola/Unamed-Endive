#include "src/game.hpp"

int main()
{
    // creating game instance, we only need this to play the game :D
    Game game {};
    int ret = game.run();// if an error occure, Game::run() will return 1, else 0 (EXIT_SUCCESS)

    // free the memory ! Alleluia
    game.quit();

    return ret;
}
