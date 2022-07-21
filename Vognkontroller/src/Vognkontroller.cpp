//
// Created by aeols on 29.04.2022.
//

#include <chrono>
#include <cstdint>

#include "Vognkontroller.h"

int main(int argc, char *argv[]) {
    Vognkontroller hub{};
    return hub.main(argc, argv);
}

int Vognkontroller::main(int argc, char *argv[]) {
    get_elapsed();
    the_logger.initialize();
    auto app = Gtk::Application::create(argc, argv, "no.åøya.dynamometer");
    optional_window = new My_window();
    if (optional_window) {
        optional_window->initialize(this, [](void *user, char key, int data) {
            if (user) ((Vognkontroller *) user)->notify(key, data);
        });
        initialize_workers();
        initialize_HID_controller();
        return app->run(*optional_window);
    } else {
        return -2;
    }
}

void Vognkontroller::initialize_HID_controller() {
    the_HID_controller.initalize(this, [](void *user, char key, int data) {
        if (user) ((Vognkontroller *) user)->notify(key, data);
    });
    optional_HID_thread = new std::thread(&My_HID_controller::run, &the_HID_controller);
}

void Vognkontroller::initialize_workers() {
    for (uint32_t gpc = 0; gpc < Number_of_workers; gpc++) {
        if (!optional_worker_thread[gpc]) {
            m_Worker[gpc].initalize(this, [](void *user, char key, int data) {
                if (user) ((Vognkontroller *) user)->notify(key, data);
            });
            optional_worker_thread[gpc] = new std::thread(&My_worker::run, &m_Worker[gpc]);
        }
    }
}

void Vognkontroller::notify(char key, int data) {
    the_logger.logg(key, data);
    switch (key) {
        case My_definitions::Manometer_hovedtrykk:
            data -= 100.0;
            if (optional_window)optional_window->set_hovedtrykk(data);
            break;
        case My_definitions::Manometer_bremsetrykk:
            if (optional_window)optional_window->set_bremsetrykk(data);
            break;
        case My_definitions::Dynamometer_trekkraft:
            if (optional_window) optional_window->set_trekkraft(-data);
            the_HID_controller.set_X(-data);
            break;
        case My_definitions::Odometer_fremover:
        case My_definitions::Odometer_bakover:
            odometer_update(data);
            break;
        case My_definitions::Button_X:
            if (optional_window) optional_window->set_flag("Way-point X");
            break;
        case My_definitions::Button_Y:
            if (optional_window) optional_window->set_flag("Way-point Y");
            break;
        case My_definitions::Button_Z:
            if (optional_window) optional_window->set_flag("Trip count 0");
            the_odometer = 0.0;
            break;
        default:
            if (optional_window) optional_window->set_flag("");
            break;
    }
}

void Vognkontroller::odometer_update(int cycles) {
    constexpr double Hjulomkrets = 0.314;
    the_odometer += Hjulomkrets;
    the_logger.logg(My_definitions::Odometer_key, the_speed);
    auto elapsed = get_elapsed();
    the_speed = (elapsed > 0) ? Hjulomkrets * 3600.0 / elapsed : 0.0;
    the_logger.logg(My_definitions::Speedometer, the_speed);
    the_HID_controller.set_Y(the_speed);
    the_HID_controller.set_Z(the_odometer);
    if (optional_window) {
        optional_window->set_omdreininger(cycles);
        optional_window->set_hastighet(the_speed);
        optional_window->set_trippteller(the_odometer);
    }
}

uint64_t Vognkontroller::get_elapsed() {
    uint64_t current = std::chrono::duration_cast<std::chrono::milliseconds>
            (std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    uint64_t elapsed = current - the_timestamp;
    the_timestamp = current;
    return elapsed;
}
