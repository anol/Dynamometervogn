//
// Created by aeols on 14.06.2022.
//

#include <string>
#include <utility>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

#include "Datasporing.h"

int Datasporing::read(const char *filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    if (file.is_open()) {
        buffer << file.rdbuf();
        auto structure = buffer.str();
        return handle_data(structure.c_str());
    } else {
        throw std::runtime_error("<> Could not open \"" + std::string(filename) + "\" <>");
    }
}

int Datasporing::handle_data(const char *data) {
    enum Line_state {
        Pos_skip, Pos_start, Pos_hrs1, Pos_hrs2, Pos_min1, Pos_min2, Pos_sec1, Pos_sec2, Pos_id, Pos_val
    };
    std::string line{};
    auto state = Pos_skip;
    while (*data) {
        auto sym = *data++;
        switch (sym) {
            default: {
                switch (state) {
                    case Pos_hrs1:
                        line.append(1, sym);
                        state = Pos_hrs2;
                        break;
                    case Pos_hrs2:
                        line.append(1, sym);
                        line.append(1, ':');
                        state = Pos_min1;
                        break;
                    case Pos_min1:
                        line.append(1, sym);
                        state = Pos_min2;
                        break;
                    case Pos_min2:
                        line.append(1, sym);
                        line.append(1, ':');
                        state = Pos_sec1;
                        break;
                    case Pos_sec1:
                        line.append(1, sym);
                        state = Pos_sec2;
                        break;
                    case Pos_sec2:
                        line.append(1, sym);
                        line.append(1, ';');
                        state = Pos_id;
                        break;
                    case Pos_id:
                        line.append(1, sym);
                        line.append(1, ';');
                        state = Pos_skip;
                        break;
                    case Pos_val:
                        line.append(1, sym);
                        break;
                    case Pos_skip:
                        break;
                }
                break;
            }
            case '.': {
                line.append(1, ',');
                break;
            }
            case ':': {
                state = Pos_id;
                break;
            }
            case '=': {
                state = Pos_val;
                break;
            }
            case ';': {
                if (state != Pos_skip) {
                    std::cout << line << std::endl;
                }
                line.clear();
                state = Pos_hrs1;
                break;
            }
            case ' ':
            case '\t': {
                break;
            }
            case '+':{
                state = Pos_skip;
                line.clear();
                break;
            }
            case '\n':
            case '\r': {
                line.clear();
                break;
            }
        }
    }
    return 0;
}
