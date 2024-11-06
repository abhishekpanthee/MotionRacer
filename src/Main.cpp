#include <iostream>
#include <cstdlib>  // For system()
#include <thread>   // For std::thread
#include <chrono>   // For std::chrono

#include "../header/Game.h"  


int main() {
    Game game;
    game.run();
    

    return 0;
}
