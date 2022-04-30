#include "My_worker.h"
#include "My_window.h"
#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <cstdio>

My_worker::My_worker(int index, const char *name) :
        the_index(index),
        the_name(name),
        m_Mutex(),
        m_shall_stop(false),
        m_has_stopped(false) {}

void My_worker::initalize(void *user, void (*func)(void *user, char key, double data)) {
    optional_user = user;
    optional_func = func;
}

void My_worker::stop_work() {
    std::lock_guard<std::mutex> lock(m_Mutex);
    m_shall_stop = true;
}

bool My_worker::has_stopped() const {
    std::lock_guard<std::mutex> lock(m_Mutex);
    return m_has_stopped;
}

void My_worker::update_data() {
    std::lock_guard<std::mutex> lock(m_Mutex);
    the_buffer[the_pos] = 0;
    try {
        auto data = std::stod(the_buffer);
        if (optional_func) {
            optional_func(optional_user, the_key, data);
        }
    }
    catch (...) {
    }
}

void My_worker::read_data(int fd) {
    char sym;
    while (read(fd, &sym, 1) > 0) {
        switch (sym) {
            case '=':
                the_pos = 0;
                break;
            case 'a':
            case 'b':
            case 'c':
            case 'd':
                the_key = sym;
                break;
            case ';':
                update_data();
                the_key = '\0';
                break;
            case '.':
                the_buffer[the_pos++] = ',';
                break;
            default:
                if ((sym >= '0') && (sym <= '9') && (the_pos < Buffer_size)) {
                    the_buffer[the_pos++] = sym;
                }
                break;
        }
    }
}

void My_worker::run() {
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_has_stopped = false;
    }
    std::cout << "\r\n Hi there " << the_name << std::endl;
    auto fd = open(the_name, O_RDWR | O_NOCTTY);
    if (fd == -1) {
        perror(the_name);
        return;
    }
    struct termios options{};
    tcgetattr(fd, &options);
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);
    tcsetattr(fd, TCSANOW, &options);
    while (!m_shall_stop) {
        read_data(fd);
    }
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_shall_stop = false;
        m_has_stopped = true;
    }
    if (optional_func) {
        optional_func(optional_user, 'x', 0.0);
    }
}
