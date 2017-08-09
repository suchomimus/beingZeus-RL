#include <string>

class Actor {
public :
    int x, y; // Actor position on console
    int ch; // Actor ASCII code
    std::string name; //character name
    TCODColor col; // Actor ACSII code color

    Actor (int x, int y, int ch, const TCODColor &col, std::string name);
    ~Actor();

    void render();
    void update();
    bool moveOrAct(int x, int y);
    void greet();

};

