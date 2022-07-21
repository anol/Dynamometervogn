//
// Created by aeols on 01.05.2022.
//

#ifndef VOGNKONTROLLER_MY_HID_CONTROLLER_H
#define VOGNKONTROLLER_MY_HID_CONTROLLER_H

#include <stdint-gcc.h>
#include <cmath>
#include "hidapi/hidapi.h"

class My_HID_controller {
    int32_t the_X{};
    int32_t the_Y{};
    int32_t the_Z{};

    void *optional_user{};

    void (*optional_func)(void *user, char key, int data){};

public:
    My_HID_controller();

    void initalize(void *user, void (*func)(void *user, char key, int data));

    int run();

    void set_X(double value) { the_X = (int) round(value * 100.0); }

    void set_Y(double value) { the_Y = (int) round(value * 100.0); }

    void set_Z(double value) { the_Z = (int) round(value * 100.0); }

private:
    int mainloop();

    void read_buttons(unsigned char *buf, hid_device *handle);

    uint8_t button_changed(uint8_t old_buttons, uint8_t new_buttons);

    void write_xyz(hid_device *handle, int32_t x, int32_t y, int32_t z);

};


#endif //VOGNKONTROLLER_MY_HID_CONTROLLER_H
