//
// Created by aeols on 01.05.2022.
//

#include <ctime>
#include <iostream>
#include <iomanip>

#include "My_filelogger.h"

My_filelogger::~My_filelogger() {
    if (the_file.is_open()) {
        the_file.close();
    }
}

void My_filelogger::initialize() {
    the_file.open("Vognkontroller.txt", std::fstream::out | std::fstream::app);
    std::time_t result = std::time(nullptr);
    the_file << ";" << std::endl;
    the_file << ";" << std::endl;
    the_file << std::put_time(std::localtime(&result), "%H%M%S:Y=%Y%m%d") << ";" << std::endl;
    the_file << ";" << std::endl;
    the_file << ";" << std::endl;
}

void My_filelogger::logg(char key, double data) {
    std::time_t result = std::time(nullptr);
    the_file << std::put_time(std::localtime(&result), "%H%M%S") << ":" << key << "=" << data << ";"
             << std::endl;
}
