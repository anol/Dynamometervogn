//
// Created by aeols on 14.06.2022.
//

#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

#include "Datasporing.h"

void Datasporing::import_entries(const char *filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    if (file.is_open()) {
        buffer << file.rdbuf();
        auto structure = buffer.str();
        handle_data(structure.c_str());
        file.close();
    } else {
        throw std::runtime_error("<> Could not open input \"" + std::string(filename) + "\" <>");
    }
}

void Datasporing::export_entries(const char *filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        serialize(file);
        file.close();
    } else {
        throw std::runtime_error("<> Could not open output \"" + std::string(filename) + "\" <>");
    }
}

void Datasporing::print_entries() {
    serialize(std::cout);
}

void Datasporing::serialize(std::ostream &out) const {
    Data_entry::serialize_statistics(out);
    the_time_series.serialize_statistics(out);
    the_time_series.serialize(out);
}

void Datasporing::handle_data(const char *data) {
    Data_entry entry{};
    while (*data) {
        if (entry.scan(*data++)) {
            the_time_series.append(entry);
            entry.clear();
        }
    }
    the_time_series.finalize();
}
