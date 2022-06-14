//
// Created by aeols on 14.06.2022.
//

#ifndef VOGNKONTROLLER_DATA_ENTRY_H
#define VOGNKONTROLLER_DATA_ENTRY_H


#include <cstdint>

class Data_entry {
    enum Line_state {
        Pos_skip, Pos_start, Pos_hrs1, Pos_hrs2, Pos_min1, Pos_min2, Pos_sec1, Pos_sec2, Pos_id, Pos_val
    };
    enum Values {
        Number_of_values = 4
    };
    Line_state the_state{};
    char the_id{};
    uint32_t the_time{};
    double the_values[Number_of_values]{};
    bool the_negatives[Number_of_values]{};
    static uint32_t the_counters[Number_of_values];

public:
    void clear();

    bool scan(char sym);

    void serialize(std::ostream &out) const;

    void append_value(char sym);

    void append_time(char sym, int multi);

    bool finalize();

    void set_id(char sym);

    static void serialize_statistics(std::ostream &out) ;
};


#endif //VOGNKONTROLLER_DATA_ENTRY_H
