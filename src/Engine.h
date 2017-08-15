#ifndef ENGINE_H
#define ENGINE_H

#include "Gui.h"

class Engine {
public :
    enum GameStatus {
        STARTUP,
        IDLE,
        NEW_TURN,
        VICTORY,
        DEFEAT
    } gameStatus;

    TCODList<Actor *> actors;
    Actor *player;
    Map *map;
    int fovRadius;
    Gui *gui;

    Engine();
    ~Engine();
    void update();
    void render();

};

extern Engine engine;

#endif // ENGINE_H