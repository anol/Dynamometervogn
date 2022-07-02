//
// Created by aeols on 01.05.2022.
//

#include <iostream>
#include <iomanip>

#include "My_filelogger.h"

My_filelogger::~My_filelogger() {
    if (the_file.is_open()) {
        the_file.close();
    }
}

void My_filelogger::initialize() {
    char filename[64] = {"vognkontroller.log"};
    the_start_time = std::time(nullptr);
    if (std::strftime(filename, sizeof(filename), "%Y-%m-%dT%H-%M-%S.log", std::localtime(&the_start_time))) {
    }
    the_file.open(filename, std::fstream::out | std::fstream::app);
    the_file << "time(ms),key,data;" << std::endl;
    the_zero_time = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch());
    logg('X', 0.0);
}

void My_filelogger::logg(char key, double data) {
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()) - the_zero_time;
    the_file << timestamp.count() << "," << key << "," << data << ";" << std::endl;
}
