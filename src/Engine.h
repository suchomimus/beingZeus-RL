#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include "Gui.h"
#include "Actor.h"
#include "Map.h"

class Engine {
public :
    enum GameStatus {
        STARTUP,
        IDLE,
        NEW_TURN,
        VICTORY,
        DEFEAT
    } gameStatus;

    TCOD_key_t lastKey;
    std::vector<std::shared_ptr<Actor>> actors;
    std::shared_ptr<Actor> player;
    const int fovRadius;
    int turn;
    const std::unique_ptr<Map> map = std::make_unique<Map>(100, 80);
    Gui *gui;

    Engine();
    ~Engine();
    void update();
    void render();
};

extern Engine engine;

#endif // ENGINE_H