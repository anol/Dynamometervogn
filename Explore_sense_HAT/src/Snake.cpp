#define DEV_INPUT_EVENT "/dev/input"
#define EVENT_DEV_NAME "event"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <time.h>
#include <poll.h>
#include <dirent.h>
#include <string.h>

#include <linux/input.h>
#include <linux/fb.h>
#include <unistd.h>

#include "Snake.h"

static int is_event_device(const struct dirent *dir) {
    return strncmp(EVENT_DEV_NAME, dir->d_name,
                   strlen(EVENT_DEV_NAME) - 1) == 0;
}

static int open_evdev(const char *dev_name) {
    struct dirent **namelist;
    int i, ndev;
    int fd = -1;

    ndev = scandir(DEV_INPUT_EVENT, &namelist, is_event_device, versionsort);
    if (ndev <= 0)
        return ndev;

    for (i = 0; i < ndev; i++) {
        char fname[64];
        char name[256];

        snprintf(fname, sizeof(fname),
                 "%s/%s", DEV_INPUT_EVENT, namelist[i]->d_name);
        fd = open(fname, O_RDONLY);
        if (fd < 0)
            continue;
        ioctl(fd, EVIOCGNAME(sizeof(name)), name);
        if (strcmp(dev_name, name) == 0)
            break;
        close(fd);
    }

    for (i = 0; i < ndev; i++)
        free(namelist[i]);

    return fd;
}

int Snake::check_collision(int appleCheck) {
    Frame_buffer::segment_t *seg_i;

    if (appleCheck) {
        for (seg_i = snake.tail; seg_i; seg_i = seg_i->next) {
            if (seg_i->x == apple.x && seg_i->y == apple.y)
                return 1;
        }
        return 0;
    }

    for (seg_i = snake.tail; seg_i->next; seg_i = seg_i->next) {
        if (snake.head.x == seg_i->x && snake.head.y == seg_i->y)
            return 1;
    }

    if (snake.head.x < 0 || snake.head.x > 7 ||
        snake.head.y < 0 || snake.head.y > 7) {
        return 1;
    }
    return 0;
}

void Snake::game_logic() {
    Frame_buffer::segment_t *seg_i;
    Frame_buffer::segment_t *new_tail;
    for (seg_i = snake.tail; seg_i->next; seg_i = seg_i->next) {
        seg_i->x = seg_i->next->x;
        seg_i->y = seg_i->next->y;
    }
    if (check_collision(1)) {
        new_tail = static_cast<Frame_buffer::segment_t *>(malloc(sizeof(Frame_buffer::segment_t)));
        if (!new_tail) {
            printf("Ran out of memory.\n");
            running = 0;
            return;
        }
        new_tail->x = snake.tail->x;
        new_tail->y = snake.tail->y;
        new_tail->next = snake.tail;
        snake.tail = new_tail;

        while (check_collision(1)) {
            apple.x = rand() % 8;
            apple.y = rand() % 8;
        }
    }
    switch (snake.heading) {
        case LEFT:
            seg_i->y--;
            break;
        case DOWN:
            seg_i->x++;
            break;
        case RIGHT:
            seg_i->y++;
            break;
        case UP:
            seg_i->x--;
            break;
    }
}

void Snake::reset() {
    Frame_buffer::segment_t *seg_i;
    Frame_buffer::segment_t *next_tail;
    seg_i = snake.tail;
    while (seg_i->next) {
        next_tail = seg_i->next;
        free(seg_i);
        seg_i = next_tail;
    }
    snake.tail = seg_i;
    snake.tail->next = NULL;
    snake.tail->x = 2;
    snake.tail->y = 3;
    apple.x = rand() % 8;
    apple.y = rand() % 8;
    snake.heading = NONE;
}

void Snake::change_dir(unsigned int code) {
    switch (code) {
        case KEY_UP:
            if (snake.heading != DOWN)
                snake.heading = UP;
            break;
        case KEY_RIGHT:
            if (snake.heading != LEFT)
                snake.heading = RIGHT;
            break;
        case KEY_DOWN:
            if (snake.heading != UP)
                snake.heading = DOWN;
            break;
        case KEY_LEFT:
            if (snake.heading != RIGHT)
                snake.heading = LEFT;
            break;
    }
}

void Snake::handle_events(int evfd) {
    struct input_event ev[64];
    int i, rd;
    rd = read(evfd, ev, sizeof(struct input_event) * 64);
    if (rd < (int) sizeof(struct input_event)) {
        fprintf(stderr, "expected %d bytes, got %d\n",
                (int) sizeof(struct input_event), rd);
        return;
    }
    for (i = 0; i < rd / sizeof(struct input_event); i++) {
        if (ev->type != EV_KEY)
            continue;
        if (ev->value != 1)
            continue;
        switch (ev->code) {
            case KEY_ENTER:
                running = 0;
                break;
            default:
                change_dir(ev->code);
        }
    }
}

int Snake::main() {
    Frame_buffer::fb_t *optional_buffer;
    int ret = 0;
    int fbfd = 0;
    struct pollfd evpoll = {
            .events = POLLIN,
    };
    srand(time(nullptr));
    evpoll.fd = open_evdev("Raspberry Pi Sense HAT Joystick");
    if (evpoll.fd < 0) {
        fprintf(stderr, "Event device not found.\n");
        return evpoll.fd;
    }
    fbfd = the_frame.open();
    if (fbfd <= 0) {
        ret = fbfd;
        printf("Error: cannot open framebuffer device.\n");
        goto err_ev;

    }
    optional_buffer = the_frame.get_buffer();
    if (!optional_buffer) {
        ret = EXIT_FAILURE;
        printf("Failed to mmap.\n");
        goto err_fb;
    }
    memset(optional_buffer, 0, 128);
    snake.tail = &snake.head;
    reset();
    while (running) {
        while (poll(&evpoll, 1, 0) > 0) {
            handle_events(evpoll.fd);
        }
        game_logic();
        if (check_collision(0)) {
            reset();
        }
        the_frame.render_snake(snake.tail, apple.x, apple.y);
        usleep(300000);
    }
    memset(optional_buffer, 0, 128);
    reset();
    munmap(optional_buffer, 128);
    err_fb:
    close(fbfd);
    err_ev:
    close(evpoll.fd);
    return ret;
}
