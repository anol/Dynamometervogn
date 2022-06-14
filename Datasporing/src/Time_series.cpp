//
// Created by aeols on 14.06.2022.
//

#include <iostream>

#include "Time_series.h"


void Time_series::serialize(std::ostream &out) const {
    for (auto &entry: the_entries) {
        entry.serialize(out);
    }
}

void Time_series::serialize_statistics(std::ostream &out) const {
    out << "Time_series";
    out << ";" << the_entries.size();
    out << ";" << the_entries.front().get_time();
    out << ";" << the_entries.back().get_time();
    out << ";" << the_append_cnt;
    out << std::endl;
}

void Time_series::append(const Data_entry &entry) {
    the_append_cnt++;
    if (entry.is_valid()) {
        if (the_entry.is_valid()) {
            if (the_entry.get_time() == entry.get_time()) {
                the_entry.merge(entry);
            } else {
                the_entries.push_back(the_entry);
                the_entry = entry;
            }
        } else {
            the_entry = entry;
        }
    }
}

void Time_series::finalize() {
    if (the_entry.is_valid()) {
        the_entries.push_back(the_entry);
        the_entry.clear();
    }
}
