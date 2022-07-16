//
// Created by aeols on 15.07.2022.
//

#include <stdexcept>
#include <system_error>
#include <cstring>
#include <unistd.h>

#include "Target_focus.h"

int Target_focus::main() {
    int fbfd = the_frame.open();
    if (fbfd <= 0) {
        throw std::runtime_error("Error: cannot open framebuffer device.\n");
    }
    auto *optional_buffer = the_frame.get_buffer();
    if (!optional_buffer) {
        throw std::runtime_error("Error: failed to mmap.\n");
    }
    the_frame.clear();
    int x = 0;
    int y = 0;
    int c = 10000;
    uint16_t color_x = 0x07E0;
    uint16_t color_y = 0xF800;
    while (c--) {
        x &= 0x7;
        y &= 0x7;
        the_frame.x_line(x++, color_x);
        the_frame.y_line(y++, color_y);
        ::usleep(30000);
    }
    the_frame.close();
    return 0;
}
