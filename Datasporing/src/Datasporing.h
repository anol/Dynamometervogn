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

#include "Data_entry.h"
#include "Time_series.h"

class Datasporing {
    Time_series the_time_series{};

public:
    void import_entries(const char *filename);

    void handle_data(const char *data);

    void export_entries(const char *filename);

    void print_entries();

private:
    void serialize(std::ostream &out) const;
};


#endif //VOGNKONTROLLER_DATASPORING_H
