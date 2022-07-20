//
// Created by aeols on 01.05.2022.
//

#ifndef VOGNKONTROLLER_MY_FILELOGGER_H
#define VOGNKONTROLLER_MY_FILELOGGER_H

#include <ctime>
#include <chrono>
#include <fstream>
#include <mutex>

class My_filelogger {
    std::fstream the_file{};
    std::time_t the_start_time{};
    std::chrono::milliseconds the_zero_time{};
    std::mutex the_mutex{};
public:
    My_filelogger() = default;

    ~My_filelogger();

    void initialize();

    void logg(char key, int data);

    void logg(char key, const std::string &data);
};


#endif //VOGNKONTROLLER_MY_FILELOGGER_H
