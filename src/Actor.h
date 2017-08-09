#include <string>

class Actor {
public :
    int x; // Actor x position on console
    int y; // Actor y position on console
    int ch; // Actor ASCII code
    std::string name; //character name
    TCODColor col; // Actocolor

    Actor (int x, int y, int ch, const TCODColor &col, std::string name);
    ~Actor();

    void render();
    void update();
    bool moveOrAct(int x, int y);
};

