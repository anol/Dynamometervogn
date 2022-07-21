//
// Created by aeols on 01.05.2022.
//

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


#include "My_HID_controller.h"
#include "My_definitions.h"

My_HID_controller::My_HID_controller() {

}

void My_HID_controller::initalize(void *user, void (*func)(void *, char, int)) {
    optional_user = user;
    optional_func = func;
}

int My_HID_controller::run() {
    while (mainloop() == 0);
    return 0;
}

int My_HID_controller::mainloop() {
    int res;
    unsigned char buf[256];
#define MAX_STR 255
    wchar_t wstr[MAX_STR];
    hid_device *handle;
    int i;
    struct hid_device_info *devs, *cur_dev;
    if (hid_init())
        return -1;
    devs = hid_enumerate(0x0, 0x0);
    cur_dev = devs;
    while (cur_dev) {
        printf("device: vid=%04hx, pid=%04hx, usage=0x%02X, page=0x%04X\n",
               cur_dev->vendor_id, cur_dev->product_id, cur_dev->usage, cur_dev->usage_page);
        cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);
    // Set up the command buffer.
    memset(buf, 0x00, sizeof(buf));
    buf[0] = 0x01;
    buf[1] = 0x81;
    // Open the device using the VID, PID,
    // and optionally the Serial number.
    ////handle = hid_open(0x4d8, 0x3f, L"12  345");
    handle = hid_open(0x6666, 0x000F, NULL);
    if (!handle) {
        printf("Unable to open device, error: %ls\n", hid_error(handle));
#ifdef WIN32
        Sleep(4000);
#else
        usleep(4000 * 1000);
#endif
        return 0;
    }
    // Read the Manufacturer String
    wstr[0] = 0x0000;
    res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
    if (res < 0)
        printf("Unable to read manufacturer string\n");
    printf("Manufacturer String: %ls\n", wstr);
    // Read the Product String
    wstr[0] = 0x0000;
    res = hid_get_product_string(handle, wstr, MAX_STR);
    if (res < 0)
        printf("Unable to read product string\n");
    printf("Product String: %ls\n", wstr);
    // Read the Serial Number String
    wstr[0] = 0x0000;
    res = hid_get_serial_number_string(handle, wstr, MAX_STR);
    if (res < 0) printf("Unable to read serial number string\n");
    printf("Serial Number String: (%d) %ls", wstr[0], wstr);
    printf("\n");
    // Set the hid_read() function to be non-blocking.
    hid_set_nonblocking(handle, 1);
    // Try to read from the device. There should be no
    // data here, but execution should not block.
    res = hid_read(handle, buf, 17);
    write_xyz(handle, the_X, the_Y, the_Z);
    read_buttons(buf, handle);
    hid_close(handle);
    /* Free static HIDAPI objects. */
    hid_exit();
    return 0;
}

void My_HID_controller::write_xyz(hid_device *handle, int32_t x, int32_t y, int32_t z) {
    unsigned char message[13] = {0x3, 0};
    message[1] = 0xFFu & ((uint32_t) x >> 24u);
    message[2] = 0xFFu & ((uint32_t) x >> 16u);
    message[3] = 0xFFu & ((uint32_t) x >> 8u);
    message[4] = 0xFFu & ((uint32_t) x);
    message[5] = 0xFFu & ((uint32_t) y >> 24u);
    message[6] = 0xFFu & ((uint32_t) y >> 16u);
    message[7] = 0xFFu & ((uint32_t) y >> 8u);
    message[8] = 0xFFu & ((uint32_t) y);
    message[9] = 0xFFu & ((uint32_t) z >> 24u);
    message[10] = 0xFFu & ((uint32_t) z >> 16u);
    message[11] = 0xFFu & ((uint32_t) z >> 8u);
    message[12] = 0xFFu & ((uint32_t) z);
    int res = hid_write(handle, message, sizeof(message));
    if (res < 0) {
        printf("Unable to write(3), error: %ls\n", hid_error(handle));
    }
}

void My_HID_controller::read_buttons(unsigned char *buf, hid_device *handle) {
    static uint8_t old_buttons = 0x00u;
    int res = 0;
    while (res >= 0) {
        res = hid_read(handle, buf, sizeof(buf));
        if ((res > 0)) {
            uint8_t new_buttons = 0x7Fu & buf[1];
            if ((old_buttons != new_buttons)) {
                old_buttons = button_changed(old_buttons, new_buttons);
                write_xyz(handle, the_X, the_Y, the_Z);
            } else if (buf[1] == 0) {
                write_xyz(handle, the_X, the_Y, the_Z);
            }
        }
        if (res == 0) {
#ifdef WIN32
            Sleep(100);
#else
            usleep(100 * 1000);
#endif
        }
        if (res < 0) printf("Unable to read()\n");
    }
}

uint8_t My_HID_controller::button_changed(uint8_t old_buttons, uint8_t new_buttons) {
    printf("%02X -> %02X\n", old_buttons, new_buttons);
    if (new_buttons == 0) {
        switch (old_buttons) {
            case 1:
                the_X = 0;
                if (optional_func) {
                    optional_func(optional_user, My_definitions::Button_X, 0);
                }
                break;
            case 2:
                the_Y = 0;
                if (optional_func) {
                    optional_func(optional_user, My_definitions::Button_Y, 0);
                }
                break;
            case 4:
                the_Z = 0;
                if (optional_func) {
                    optional_func(optional_user, My_definitions::Button_Z, 0);
                }
                break;
            default:
                break;
        }
    } else if (new_buttons == 1 && old_buttons == 3) {
        if (optional_func) {
            optional_func(optional_user, My_definitions::Button_XY, 0);
        }
    } else if (new_buttons == 1 && old_buttons == 5) {
        if (optional_func) {
            optional_func(optional_user, My_definitions::Button_XZ, 0);
        }
    }
    return new_buttons;
}
