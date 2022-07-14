//
// Created by aeols on 14.07.2022.
//

#ifndef EXPLORE_SENSE_HAT_SNAKE_H
#define EXPLORE_SENSE_HAT_SNAKE_H

#include "Frame_buffer.h"

class Snake {
    enum direction_t {
        UP, RIGHT, DOWN, LEFT, NONE,
    };

    struct snake_t {
        struct Frame_buffer::segment_t head;
        struct Frame_buffer::segment_t *tail;
        enum direction_t heading;
    };

    struct apple_t {
        int x;
        int y;
    };

    Frame_buffer the_frame{};
    snake_t snake = {{nullptr, 4, 4}, nullptr, NONE,};
    int running{1};
    apple_t apple{4, 4,};

public:
    int main();

private:
    int check_collision(int appleCheck);

    void game_logic();

    void reset();

    void change_dir(unsigned int code);

    void handle_events(int evfd);
};

#endif //EXPLORE_SENSE_HAT_SNAKE_H
