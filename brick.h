#include <iostream>
#include <vector>
#include <cstdlib>

#ifndef BRICK_H
#define BRICK_H

class brick {
    public:
        bool stuck;
        int x, y, color;
        brick(int colour);
        brick(int x, int y);
        brick();
};

class brickbuddle {
    public:
    int type;
    int direction;
    void set_direction();
    brick bricks[4];
    brickbuddle(int col, int row);
    brickbuddle();
};

#endif
