//
// Created by Ryan C Brown on 8/14/17.
//

#include <cstring>
#include "main.h"


static const int PANEL_HEIGHT=15;
static const int BAR_WIDTH=22;
static const int MSG_X=BAR_WIDTH+2;
static const int MSG_HEIGHT=PANEL_HEIGHT-1;

Gui::Gui() {
    con = new TCODConsole(100,PANEL_HEIGHT);
}

Gui::~Gui() {
    delete con;
    log.clearAndDelete();
}

Gui::Message::Message(const char *text, const TCODColor &col) :
        col(col) {
    this->text = new char[strlen(text)];
    strcpy(this->text,text);
}

Gui::Message::~Message() {
    delete [] text;
}

void Gui::render() {
    con->setDefaultBackground(TCODColor::black);
    con->clear();
    renderBar(1,1,BAR_WIDTH,"Love Points", engine.player->destructible->curHP, engine.player->destructible->maxHP,
              TCODColor::lightRed, TCODColor::darkerRed);
    int y=1;
    float colorCoef=0.4f;
    for (Message **it=log.begin(); it != log.end(); it++) {
        Message *message=*it;
        con->setDefaultForeground(message->col * colorCoef);
        con->print(MSG_X,y,message->text);
        y++;
        if ( colorCoef < 1.0f ) {
            colorCoef+=0.3f;
        }
    }
    TCODConsole::blit(con,0,0,100,PANEL_HEIGHT,TCODConsole::root,0,95-PANEL_HEIGHT);
}


void Gui::renderBar(int x, int y, int width, const char *name, float value, float maxValue, const TCODColor &barColor,
                    const TCODColor &backColor) {

    con->setDefaultBackground(backColor);
    con->rect(x, y, width, 1, false, TCOD_BKGND_SET);

    auto barWidth = int(value / maxValue * width);
    if (barWidth > 0) {
        con->setDefaultBackground(barColor);
        con->rect(x, y, barWidth, 1, false, TCOD_BKGND_SET);
        con->setDefaultForeground(TCODColor::white);
        con->printEx(x+width/2,y,TCOD_BKGND_NONE, TCOD_CENTER,
        "%s : %g/%g", name, value, maxValue);
    }
    else {
        con->setDefaultBackground(barColor);
        con->rect(x, y, barWidth, 1, false, TCOD_BKGND_SET);
        con->setDefaultForeground(TCODColor::white);
        con->printEx(x+width/2,y,TCOD_BKGND_NONE, TCOD_CENTER,
                     "Zeus needs love!");
    }


}

void Gui::message(const TCODColor &col, const char *text, ...) {
    // build the text
    va_list ap;
    char buf[128];
    va_start(ap,text);
    vsprintf(buf,text,ap);
    va_end(ap);

    char *lineBegin=buf;
    char *lineEnd;
    do {
        // make room for the new message
        if ( log.size() == MSG_HEIGHT ) {
            Message *toRemove=log.get(0);
            log.remove(toRemove);
            delete toRemove;
        }

        // detect end of the line
        lineEnd=strchr(lineBegin,'\n');
        if ( lineEnd ) {
            *lineEnd='\0';
        }

        // add a new message to the log
        Message *msg=new Message(lineBegin, col);
        log.push(msg);

        // go to next line
        lineBegin=lineEnd+1;
    } while ( lineEnd );
}



