//
// Created by Leon on 4/20/2019.
//

#ifndef ALPACAWEB_GAME_H
#define ALPACAWEB_GAME_H

#include "Window.h"


class Game {
public:
    void run();
private:
    Window window;
    void mainLoop();
};


#endif //ALPACAWEB_GAME_H
