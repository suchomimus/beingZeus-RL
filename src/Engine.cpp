#include <iostream>
#include "libtcod.hpp"
#include "Actor.h"
#include "Map.h"
#include "Engine.h"


Engine::Engine() : gameStatus(STARTUP), fovRadius(15) {
  TCODConsole::initRoot(110,110,"Being Zeus",false);
  player = new Actor(40,25,'d',TCODColor::lighterSepia, "Zeus");
  actors.push(player);
  map = new Map(100,100);
}

Engine::~Engine() {
  actors.clearAndDelete();
  delete map;
}

void Engine::update() {
    TCOD_key_t key;
    if ( gameStatus == STARTUP ) {
        map->computeFov();
    }
    gameStatus = IDLE;
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, nullptr);
    int dx = 0;
    int dy = 0;
    switch(key.vk) {
        case TCODK_UP : dy = -1; break;
        case TCODK_DOWN : dy = 1; break;
        case TCODK_LEFT : dx = -1; break;
        case TCODK_RIGHT : dx = 1; break;
        default:break;
    }
    if (dx != 0 || dy != 0) {
        gameStatus = NEW_TURN;
        if (player->moveOrAct(player->x+dx, player->y+dy)){
            map->computeFov();
        }
    }
    if (gameStatus == NEW_TURN){
        for (auto i : engine.actors){
            Actor *actor = i;
            if (actor != player) {
                actor->update();
            }
        }
    }
}

void Engine::render() {
    // clear the console, draw map and all actors
    TCODConsole::root->clear();
    map->render();
    for(auto i : actors) {
        Actor *actor = i;
        if (map->isInFov(actor->x, actor->y)) {
            actor->render();
        }
    }
}
