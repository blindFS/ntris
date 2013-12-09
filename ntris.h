#include "mwin.h"

#ifndef NTRIS
#define NTRIS

#define INTERVAL 100000
#define REFRESH 10

class ntris {
    bool running;
    mwin mw;
    int count, score;
    brickbuddle current, next;
    brick gbricks[MHEIGHT+1][MWIDTH/2];
    int speed;
    void set_speed(int spd);
    void hit_ground();
    void vanish();
    void main_refresh();
    void show_next();
    void show_score();
    void game_contine();
    void brick_drop();
    void brick_move(char op);
    void get_key();
    void stop_if_necessary();
    brickbuddle* check_movable(char op);
    public:
    ntris();
    void game_start();
};

#endif
