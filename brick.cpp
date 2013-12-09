#include "brick.h"

// The offsets of the other 3 bricks(relative to the rotate center) in a buddle
static int brick_matrix[7][3][2]= {
    {            // x
        {0, -1}, // %
        {0, 1},  // x
        {0, 2}   // x
    },
    {
        {0, 1}, // %x
        {1, 0}, // xx
        {1, 1}
    },
    {
        {0, -1}, //  xx
        {-1, 0}, // x%
        {1, -1}
    },
    {
        {-1, -1}, // xx
        {0, -1},  //  %x
        {1, 0}
    },
    {
        {1, 0},   // x%x
        {-1, 0},  //   x
        {1, 1}
    },
    {
        {1, 0},   // x%x
        {-1, 0},  // x
        {-1, 1}
    },
    {
        {0, -1},  //  x
        {1, 0},   // x%x
        {-1, 0}
    }
};

// x'=m[0][0]*x+m[0][1]*y; y'=m[1][0]*x+m[1][1]*y
static int brick_direction[4][2][2] = {
    {
        {1, 0},
        {0, 1}
    },
    {
        {0, -1},
        {1, 0}
    },
    {
        {-1, 0},
        {0, -1}
    },
    {
        {0, 1},
        {-1, 0}
    }
};

brick::brick(int colour) {
    x = 0;
    y = 0;
    color = colour;
    stuck = false;
}

brick::brick() {
    x = 0;
    y = 0;
    color = 0;
    stuck = false;
}

brick::brick(int col, int row) {
    x = col;
    y = row;
    color = 0;
    stuck = false;
}

brickbuddle::brickbuddle(int col, int row) {
    srand(time(0));
    type = rand() % 7;
    direction = rand() % 4;
    for (int i = 0; i < 4; i++)
        bricks[i] = brick(type+1);
    bricks[0].x = col;
    bricks[0].y = row;
    set_direction();
}

brickbuddle::brickbuddle() {
    srand(time(0));
    type = rand() % 7;
    direction = rand() % 4;
    for (int i = 0; i < 4; i++)
        bricks[i] = brick(type+1);
    bricks[0].x = 3;
    bricks[0].y = 3;
    set_direction();
}

void brickbuddle::set_direction() {
    for (int i = 1; i < 4; i++) {
        int offsetx = brick_direction[direction][0][0]*brick_matrix[type][i-1][0]+brick_direction[direction][0][1]*brick_matrix[type][i-1][1];
        int offsety = brick_direction[direction][1][0]*brick_matrix[type][i-1][0]+brick_direction[direction][1][1]*brick_matrix[type][i-1][1];
        bricks[i].x = bricks[0].x+offsetx;
        bricks[i].y = bricks[0].y+offsety;
    }
}
