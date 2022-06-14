//
// Created by aeols on 14.06.2022.
//

#ifndef VOGNKONTROLLER_DATASPORING_H
#define VOGNKONTROLLER_DATASPORING_H

#ifdef _WIN32

#include <windows.h>

#else
#include <unistd.h>
#endif

class Datasporing {

public:
    int read(const char *filename);

    int handle_data(const char *data);
};


#endif //VOGNKONTROLLER_DATASPORING_H
