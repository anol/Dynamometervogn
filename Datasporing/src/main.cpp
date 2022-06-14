//
// Created by aeols on 14.06.2022.
//
#include <stdexcept>
#include <iostream>

#include "Datasporing.h"

int main(int argc, char *argv[]) {
    try {
        Datasporing app{};
        if (argc > 1) {
            app.import_entries(argv[1]);
        } else {
            throw std::runtime_error("<> Missing file name <>");
        }
        if (argc > 2) {
            app.export_entries(argv[2]);
        } else {
            app.print_entries();
        }
        return 0;
    } catch (const std::runtime_error &ex) {
        std::cout << ex.what() << std::endl;
    }
}