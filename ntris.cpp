#include "ntris.h"
#include <unistd.h>

ntris::ntris() {
    mw = mwin();
    running = true;
    current = brickbuddle(7, 1);
    next = brickbuddle();
    count = 0;
    score = 0;
    speed = 40;
    for (int i = 0; i < MHEIGHT+1; i++) {
        for (int j = 0; j < MWIDTH/2; j++) {
            gbricks[i][j] = brick(i, 2*j);
        }
    }
    for (int j = 0; j < MWIDTH/2; j++)
        gbricks[MHEIGHT][j].stuck = true;
}

void ntris::game_start() {

    this->show_next();
    this->show_score();
    nodelay(stdscr, true);
    while (running) {
        usleep(INTERVAL);
        this->get_key();
    }
}

void ntris::game_contine() {
    count++;
    this->main_refresh();
    if (count*speed > REFRESH) {
        this->stop_if_necessary();
        this->brick_move('f');
        count = 0;
    }
}

void ntris::show_next() {
    werase(mw.previewwin);
    wattron(mw.previewwin, COLOR_PAIR(next.type+1));
    for (int i = 0; i < 4; i++)
        mvwprintw(mw.previewwin, next.bricks[i].y+1, next.bricks[i].x*2+1, "  ");
    wattroff(mw.previewwin, COLOR_PAIR(next.type+1));
    box(mw.previewwin, 0, 0);
    wrefresh(mw.previewwin);
}

void ntris::brick_move(char op) {
    int x, y;
    brickbuddle* tmp = this->check_movable(op);
    if (tmp != NULL) {
        for (int i = 0; i < 4; i++) {
            x = current.bricks[i].x;
            y = current.bricks[i].y;
            if (x >= 0 && y >= 0 && x < MWIDTH/2 && y <= MHEIGHT) {
                gbricks[y][x].color = 0;
            }
        }
        current = *tmp;
        for (int i = 0; i < 4; i++) {
            x = current.bricks[i].x;
            y = current.bricks[i].y;
            gbricks[y][x].color = current.type+1;
        }
        this->main_refresh();
    }
}

brickbuddle* ntris::check_movable(char op) {
    brickbuddle* newb;
    brickbuddle tmp = current;
    newb = &tmp;
    switch (op) {
        case 'r':
            newb->direction = (newb->direction+1)%4;
            newb->set_direction();
            for (int i = 0; i < 4; i++) {
                if (newb->bricks[i].x < 0 || newb->bricks[i].x >= MWIDTH/2)
                    return NULL;
                if (gbricks[newb->bricks[i].y][newb->bricks[i].x].stuck == true) {
                    return NULL;
                }
            }
            break;
        case 'f': // fall
            for (int i = 0; i < 4; i++) {
                newb->bricks[i].y += 1;
            }
            break;
        case 'd': // down
            for (int i = 0; i < 4; i++) {
                newb->bricks[i].y += 1;
            }
            break;
        case '<': // left
            for (int i = 0; i < 4; i++) {
                newb->bricks[i].x -= 1;
            }
            for (int i = 0; i < 4; i++) {
                if(newb->bricks[i].x < 0)
                    return NULL;
                if (gbricks[newb->bricks[i].y][newb->bricks[i].x].stuck == true) {
                    return NULL;
                }
            }
            break;
        case '>': // right
            for (int i = 0; i < 4; i++) {
                newb->bricks[i].x += 1;
            }
            for (int i = 0; i < 4; i++) {
                if(newb->bricks[i].x == MWIDTH/2)
                    return NULL;
                if (gbricks[newb->bricks[i].y][newb->bricks[i].x].stuck == true) {
                    return NULL;
                }
            }
            break;
    }
    return newb;
}

void ntris::get_key() {
    switch (getch()) {
        case 'h':
            this->brick_move('<');
            break;
        case 'j':
            this->brick_move('r');
            break;
        case 'k':
            this->stop_if_necessary();
            this->brick_move('d');
            break;
        case 'l':
            this->brick_move('>');
            break;
        default:
            this->game_contine();
    }
    flushinp();
}

void ntris::main_refresh() {
    for (int i = 0; i < MHEIGHT; i++) {
        for (int j = 0; j < MWIDTH/2; j++) {
            wattron(mw.mainwin, COLOR_PAIR(gbricks[i][j].color));
            mvwprintw(mw.mainwin, i+1, j*2+1, "  ");
            wattroff(mw.mainwin, COLOR_PAIR(gbricks[i][j].color));
        }
    }
    wrefresh(mw.mainwin);
}

void ntris::stop_if_necessary() {
    int x, y;
    for (int i = 0; i < 4; i++) {
        x = current.bricks[i].x;
        y = current.bricks[i].y;
        if (gbricks[y+1][x].stuck) {
            for (int j = 0; j < 4; j++) {
                x = current.bricks[j].x;
                y = current.bricks[j].y;
                gbricks[y][x].stuck = true;
            }
            if (current.bricks[0].y < 5) {
                running = false;
            }
            current = next;
            current.bricks[0].x = 7;
            current.bricks[0].y = 1;
            current.set_direction();
            next = brickbuddle();
            this->show_next();
            this->vanish();
            return;
        }
    }
}

void ntris::vanish() {
    int lines = 0;
    for (int i = 29; i >= 0; i--) {
        bool full = true;
        while(full) {
            for (int j = 0; j < MWIDTH/2; j++) {
                if (gbricks[i][j].stuck == false) {
                    full = false;
                    break;
                }
            }
            if (full) {
                lines++;
                for (int k = i-1; k > 0; k--) {
                    for (int j = 0; j < MWIDTH/2; j++)
                        gbricks[k+1][j] = gbricks[k][j];
                }
            }
        }
    }
    score += lines*lines;
    this->show_score();
}

void ntris::show_score()
{
    werase(mw.scorewin);
    mvwprintw(mw.scorewin, 2, 2, "score:%d", score);
    mvwprintw(mw.scorewin, 4, 2, "speed:%d", speed);
    box(mw.previewwin, 0, 0);
    wrefresh(mw.scorewin);
}
