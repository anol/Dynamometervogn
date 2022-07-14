//
// Created by aeols on 14.07.2022.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <linux/input.h>
#include <linux/fb.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/mman.h>

#include "Frame_buffer.h"

#define DEV_FB "/dev"
#define FB_DEV_NAME "fb"

static int is_framebuffer_device(const struct dirent *dir) {
    return strncmp(FB_DEV_NAME, dir->d_name,
                   strlen(FB_DEV_NAME) - 1) == 0;
}

int Frame_buffer::open() {
    constexpr char dev_name[] = "RPi-Sense FB";
    struct dirent **namelist;
    int i;
    the_fd = -1;
    struct fb_fix_screeninfo fix_info{};

    int ndev = scandir(DEV_FB, &namelist, is_framebuffer_device, versionsort);
    if (ndev <= 0)
        return ndev;

    for (i = 0; i < ndev; i++) {
        char fname[64];
        char name[256];

        snprintf(fname, sizeof(fname),
                 "%s/%s", DEV_FB, namelist[i]->d_name);
        the_fd = ::open(fname, O_RDWR);
        if (the_fd < 0)
            continue;
        ::ioctl(the_fd, FBIOGET_FSCREENINFO, &fix_info);
        if (strcmp(dev_name, fix_info.id) == 0)
            break;
        ::close(the_fd);
        the_fd = -1;
    }
    for (i = 0; i < ndev; i++)
        free(namelist[i]);
    if (the_fd) {
        optional_buffer = static_cast<fb_t *>(mmap(0, 128, PROT_READ | PROT_WRITE, MAP_SHARED, the_fd, 0));
    }
    return the_fd;
}

void Frame_buffer::render_snake(Frame_buffer::segment_t *segment_tail, int x, int y) {
    struct segment_t *seg_i;
    memset(optional_buffer, 0, 128);
    optional_buffer->pixel[x][y] = 0xF800;
    for (seg_i = segment_tail; seg_i->next; seg_i=seg_i->next) {
        optional_buffer->pixel[seg_i->x][seg_i->y] = 0x7E0;
    }
    optional_buffer->pixel[seg_i->x][seg_i->y] = 0xFFFF;
}
