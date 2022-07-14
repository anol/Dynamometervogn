//
// Created by aeols on 14.07.2022.
//

#ifndef SNAKE_FRAME_BUFFER_H
#define SNAKE_FRAME_BUFFER_H

#include <stdint.h>

class Frame_buffer {
public:
    struct fb_t {
        uint16_t pixel[8][8];
    };

private:
    fb_t *optional_buffer{};
    int the_fd{};

public:
    struct segment_t {
        struct segment_t *next;
        int x;
        int y;
    };

public:
    int open();

    void render_snake(segment_t *segment_tail, int x, int y);

    fb_t *get_buffer() { return optional_buffer; };
};


#endif //SNAKE_FRAME_BUFFER_H
