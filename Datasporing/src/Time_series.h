//
// Created by aeols on 14.06.2022.
//

#ifndef VOGNKONTROLLER_TIME_SERIES_H
#define VOGNKONTROLLER_TIME_SERIES_H

#include <vector>

#include "Data_entry.h"

class Time_series {
    Data_entry the_entry{};
    uint32_t the_append_cnt{};
    std::vector<Data_entry> the_entries{};

public:
    void serialize(std::ostream &out) const;

    void serialize_statistics(std::ostream &out) const;

    void append(const Data_entry &entry);

    void finalize();
};


#endif //VOGNKONTROLLER_TIME_SERIES_H
