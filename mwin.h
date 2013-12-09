#include "brick.h"
#include <ncurses.h>

#ifndef MWIN_H
#define MWIN_H
#define WIDTH 80
#define HEIGHT 40
#define MWIDTH 30
#define MHEIGHT 30

class mwin {
    public:
    WINDOW* mainwin;
    WINDOW* previewwin;
    WINDOW* scorewin;
    mwin();
    void newbundle();
};

#endif
