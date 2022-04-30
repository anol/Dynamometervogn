//
// Created by aeols on 29.04.2022.
//


//#include <cstdio>
//#include <cstring>
//#include <unistd.h>
//#include <iostream>
//
//using namespace std;

#include "Dynamometer.h"
#include "My_window.h"

int main(int argc, char *argv[]) {
    Dynamometer hub{};
    return hub.start_hub(argc, argv);
}

int Dynamometer::start_hub(int argc, char *argv[]) {
    auto app = Gtk::Application::create(argc, argv, "no.åøya.dynamometer");
    My_window helloworld;
    helloworld.initialize();
    return app->run(helloworld);
}
