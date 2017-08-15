//
// Created by Ryan C Brown on 8/14/17.
//

#ifndef BZ_GUI_H
#define BZ_GUI_H

#include <libtcod.hpp>


class Gui {
public:
    Gui();
    ~Gui();
    void render();
    void message(const TCODColor &col, const char *text, ...);


protected:
    TCODConsole *con;
    struct Message {
        char *text;
        TCODColor col;
        Message(const char *text, const TCODColor &col);
        ~Message();
    };
    TCODList<Message *> log;
    void renderBar(int x, int y, int width, const char *name,
    float value, float maxValue, const TCODColor &barcolor, const TCODColor &backColor);
};


#endif //BZ_GUI_H
