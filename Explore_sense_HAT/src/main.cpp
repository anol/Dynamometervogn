//
// Created by aeols on 14.07.2022.
//

#include <iostream>
#include <exception>

#include "Snake.h"
#include "Target_focus.h"

int main(int argc, char *argv[]) {
    try {
        //    Snake app;
        Target_focus app;
        return app.main();
    }
    catch (std::exception &e) {
        std::cout << e.what() << '\n';
    }
}
