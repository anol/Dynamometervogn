//
// Created by aeols on 14.06.2022.
//

#include <iostream>

#include "Data_entry.h"

uint32_t Data_entry::the_counters[]{};

void Data_entry::clear() {
    the_id = ' ';
    the_time = 0;
}

void Data_entry::serialize(std::ostream &out) const {
    out << the_time;
    for (auto &value: the_values) {
        out << ";" << value;
    }
    out << std::endl;
}

void Data_entry::serialize_statistics(std::ostream &out) {
    out << "Counters:";
    for (auto &counter: the_counters) {
        out << ";" << counter;
    }
    out << std::endl;
}

bool Data_entry::finalize() {
    for (auto gpc = 0; gpc < Number_of_values; gpc++) {
        if(the_values[gpc] > 10000.0){
            the_values[gpc] = 0.1;
        }
        if (the_negatives[gpc] && the_values[gpc] > 0.0) {
            the_values[gpc] = -the_values[gpc];
        }
    }
    return true;
}

void Data_entry::set_id(char sym) {
    the_id = sym;
    auto index = (the_id >= 'a') ? (the_id - 'a') : Number_of_values;
    if (index < Number_of_values) {
        the_counters[index]++;
        the_values[index] = 0.0;
        the_negatives[index] = false;
    }
}

void Data_entry::append_value(char sym) {
    auto index = (the_id >= 'a') ? (the_id - 'a') : Number_of_values;
    if (index < Number_of_values) {
        if (('0' <= sym) && ('9' >= sym)) {
            the_values[index] *= 10.0;
            the_values[index] += (sym - '0');
        } else if ('-' <= sym) {
            the_negatives[index] = true;
        }
    }
}

void Data_entry::append_time(char sym, int multi) {
    if (('0' <= sym) && ('9' >= sym) && (0 < multi)) {
        the_time += (sym - '0') * multi;
    }
}

bool Data_entry::scan(char sym) {
    bool entry_is_complete{};
    switch (sym) {
        default: {
            switch (the_state) {
                case Pos_hrs1:
                    append_time(sym, 36000);
                    the_state = Pos_hrs2;
                    break;
                case Pos_hrs2:
                    append_time(sym, 3600);
                    the_state = Pos_min1;
                    break;
                case Pos_min1:
                    append_time(sym, 600);
                    the_state = Pos_min2;
                    break;
                case Pos_min2:
                    append_time(sym, 60);
                    the_state = Pos_sec1;
                    break;
                case Pos_sec1:
                    append_time(sym, 10);
                    the_state = Pos_sec2;
                    break;
                case Pos_sec2:
                    append_time(sym, 1);
                    the_state = Pos_id;
                    break;
                case Pos_id:
                    set_id(sym);
                    the_state = Pos_skip;
                    break;
                case Pos_val:
                    append_value(sym);
                    break;
                case Pos_skip:
                case Pos_start:
                    break;
            }
            break;
        }
        case '.': {
            break;
        }
        case ':': {
            the_state = Pos_id;
            break;
        }
        case '=': {
            the_state = Pos_val;
            break;
        }
        case ';': {
            if (the_state != Pos_skip) {
                entry_is_complete = finalize();
            }
            the_state = Pos_hrs1;
            break;
        }
        case ' ':
        case '\t': {
            break;
        }
        case '+': {
            clear();
            the_state = Pos_skip;
            break;
        }
        case '\n':
        case '\r': {
            clear();
            break;
        }
    }
    return entry_is_complete;
}
