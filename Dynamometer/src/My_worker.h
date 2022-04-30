//
// Created by aeols on 24.09.2021.
//

#ifndef PINATOR_TEST_MY_WORKER_H
#define PINATOR_TEST_MY_WORKER_H

#include <gtkmm.h>
#include <thread>
#include <mutex>
#include <stdint-gcc.h>

class Dynamometer;

class My_worker {
    int the_index;
    const char *the_name;
    enum {
        Number_of_values = 4,
        Buffer_size = 16
    };
    char the_key{};
    char the_buffer[Buffer_size]{};
    int the_pos{};
    void *optional_user{};

    void (*optional_func)(void *user, char key, double data){};

public:
    My_worker(int index, const char *name);

    void run();

    void stop_work();

    bool has_stopped() const;

    void initalize(void *user, void (*func)(void *user, char key, double data));

private:
    void update_data();

    void read_data(int fd);

private:
    // Synchronizes access to member data.
    mutable std::mutex m_Mutex;

    // Data used by both GUI thread and worker thread.
    bool m_shall_stop;
    bool m_has_stopped;
};


#endif //PINATOR_TEST_MY_WORKER_H
