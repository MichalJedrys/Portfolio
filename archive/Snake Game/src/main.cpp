#include "Game.hpp"

int main() {
    Console console;
    Game game(console);
    while(!console.exitGame())
    {
        if(!game.isGameOver()) {
            game.run();
        }else{
            console.menu(game.getScore());
            game.restart();
        }
    }
    return 0;
}
