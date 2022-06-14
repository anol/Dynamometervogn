//
// Created by aeols on 14.06.2022.
//
#include <stdexcept>
#include <iostream>

#include "Datasporing.h"

int main(int argc, char *argv[]) {
    try {
        if (argc > 1) {
            Datasporing app{};
            return app.read(argv[1]);
        } else {
            throw std::runtime_error("<> Missing file name <>");
        }
    } catch (const std::runtime_error &ex) {
        std::cout << ex.what() << std::endl;
    }
}