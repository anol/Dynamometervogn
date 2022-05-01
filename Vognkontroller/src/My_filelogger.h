//
// Created by aeols on 01.05.2022.
//

#ifndef VOGNKONTROLLER_MY_FILELOGGER_H
#define VOGNKONTROLLER_MY_FILELOGGER_H

#include <fstream>

class My_filelogger {
    std::fstream the_file{};

public:
    My_filelogger() = default;

    ~My_filelogger();

    void initialize();

    void logg(char key, double data);
};


#endif //VOGNKONTROLLER_MY_FILELOGGER_H
