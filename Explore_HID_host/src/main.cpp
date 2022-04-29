/*******************************************************
 Windows HID simplification
 Alan Ott
 Signal 11 Software
 8/22/2009
 Copyright 2009
 This contents of this file may be used by anyone
 for any reason without any conditions and may be
 used as a starting point for your own applications
 which use HIDAPI.
********************************************************/
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include "hidapi/hidapi.h"
// Headers needed for sleeping.
#ifdef _WIN32

#include <windows.h>
#include <cstdint>

#else
#include <unistd.h>
#include <cstdint>
#endif

int32_t my_x = 12345678;

int32_t my_y = 100;

int32_t my_z = -100;

void read_buttons(unsigned char *buf, hid_device *handle);

void button_changed(uint8_t old_buttons, uint8_t new_buttons);

void write_xyz(hid_device *handle, int32_t x, int32_t y, int32_t z);

static int main_loop();

int main(int argc, char *argv[]) {
#ifdef WIN32
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);
#endif
    while(main_loop()==0);
}

int main_loop() {
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
        usleep(4000*1000);
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
    if (res < 0)
        printf("Unable to read serial number string\n");
    printf("Serial Number String: (%d) %ls", wstr[0], wstr);
    printf("\n");
//    // Read Indexed Strings
//    res = 0;
//    for (int cnt = 1; cnt < 6 && res >= 0; cnt++) {
//        wstr[0] = 0x0000;
//        res = hid_get_indexed_string(handle, cnt, wstr, MAX_STR);
//        if (res < 0)
//            printf("Unable to read indexed string %d\n", cnt);
//        else
//            printf("Indexed String %d: %ls\n", cnt, wstr);
//    }
    // Set the hid_read() function to be non-blocking.
    hid_set_nonblocking(handle, 1);
    // Try to read from the device. There should be no
    // data here, but execution should not block.
    res = hid_read(handle, buf, 17);
//    // Send a Feature Report to the device
//    buf[0] = 0x2;
//    buf[1] = 0xa0;
//    buf[2] = 0x0a;
//    buf[3] = 0x00;
//    buf[4] = 0x00;
//    res = hid_send_feature_report(handle, buf, 17);
//    if (res < 0) {
//        printf("Unable to send a feature report.\n");
//        printf("Error: %ls\n", hid_error(handle));
//    }
//    memset(buf, 0, sizeof(buf));
//    // Read a Feature Report from the device
//    buf[0] = 0x2;
//    res = hid_get_feature_report(handle, buf, sizeof(buf));
//    if (res < 0) {
//        printf("Unable to get a feature report.\n");
//        printf("Error: %ls\n", hid_error(handle));
//    } else {
//        // Print out the returned buffer.
//        printf("Feature Report\n   ");
//        for (i = 0; i < res; i++)
//            printf("%02hhx ", buf[i]);
//        printf("\n");
//    }
//    memset(buf, 0, sizeof(buf));
//    // Toggle LED (cmd 0x80). The first byte is the report number (0x1).
//    buf[0] = 0x2;
//    buf[1] = 0x80;
//    res = hid_write(handle, buf, 17);
//    if (res < 0) {
//        printf("Unable to write(2)\n");
//        printf("Error: %ls\n", hid_error(handle));
//    }
    write_xyz(handle, my_x++, my_y++, my_z++);
    read_buttons(buf, handle);
    hid_close(handle);
    /* Free static HIDAPI objects. */
    hid_exit();
//#ifdef WIN32
//    system("pause");
//#endif
    return 0;
}

void write_xyz(hid_device *handle, int32_t x, int32_t y, int32_t z) {
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

void read_buttons(unsigned char *buf, hid_device *handle) {
    static uint8_t old_buttons = 0x00u;
    int res = 0;
    while (res >= 0) {
        res = hid_read(handle, buf, sizeof(buf));
        if ((res > 0)) {
            uint8_t new_buttons = 0x7Fu & buf[1];
            if ((old_buttons != new_buttons)) {
                button_changed(old_buttons, new_buttons);
                old_buttons = new_buttons;
                write_xyz(handle, my_x++, my_y++, my_z++);
            } else if (buf[1] == 0) {
                write_xyz(handle, my_x++, my_y++, my_z++);
            }
        }
        if (res == 0) {
#ifdef WIN32
            Sleep(100);
#else
            usleep(100*1000);
#endif
        }
        if (res < 0) printf("Unable to read()\n");
    }
}

void button_changed(uint8_t old_buttons, uint8_t new_buttons) {
    printf("%02X -> %02X\n", old_buttons, new_buttons);
    if (new_buttons == 0) {
        switch (old_buttons) {
            case 1:
                my_x = 0;
                break;
            case 2:
                my_y = 0;
                break;
            case 4:
                my_z = 0;
                break;
            default:
                break;
        }
    }
}
