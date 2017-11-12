#include <iostream>
#include "main.h"


Engine::Engine() : gameStatus(STARTUP), fovRadius(15) {
    TCODConsole::initRoot(110,100,"Being Zeus",false);
    player = std::make_shared<Actor>(40,25,'d',TCODColor::lighterSepia, "Zeus");
    player->isPlayer = true;
    player->destructible = new PlayerDestructible(100,5,"your cadaver");
    player->attacker = new Attacker(5);
    player->ai = new PlayerAi();
    // player starting position
    player->x = 20;
    player->y = 20;

    actors.push_back(player);
    gui = new Gui();
    gui->message(TCODColor::lighterSepia,
                 "Good Morning Zeus!");
}

Engine::~Engine() {
    delete gui;
}

void Engine::update() {
    if ( gameStatus == STARTUP ) {
        map->computeFov();
    }
    gameStatus = IDLE;
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &lastKey, nullptr);
    player->update();
    if (gameStatus == NEW_TURN){
        for (const auto &actor : engine.actors){
            //update all actors in FoV who is not the player
            if (!actor->isPlayer && engine.map->isInFov(actor->x, actor->y)) {
                actor->update();
                std::cout << "actor ref count " << actor.use_count() << "\n";

            }
        }
    }
}

void Engine::render() {
    // clear the console, draw map and all actors
    TCODConsole::root->clear();
    map->render();
    gui->render();
    for(const auto &actor : actors) {
        if (map->isInFov(actor->x, actor->y)) {
            actor->render();
        }
    }
}

