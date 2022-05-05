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
        optional_window->initialize();
        initialize_workers();
        initialize_HID_controller();
        return app->run(*optional_window);
    } else {
        return -2;
    }
}

void Vognkontroller::initialize_HID_controller() {
    the_HID_controller.initalize(this, [](void *user, char key, double data) {
        if (user) ((Vognkontroller *) user)->notify(key, data);
    });
    optional_HID_thread = new std::thread(&My_HID_controller::run, &the_HID_controller);
}

void Vognkontroller::initialize_workers() {
    for (uint32_t gpc = 0; gpc < Number_of_workers; gpc++) {
        if (!optional_worker_thread[gpc]) {
            m_Worker[gpc].initalize(this, [](void *user, char key, double data) {
                if (user) ((Vognkontroller *) user)->notify(key, data);
            });
            optional_worker_thread[gpc] = new std::thread(&My_worker::run, &m_Worker[gpc]);
        }
    }
}

void Vognkontroller::notify(char key, double data) {
    the_logger.logg(key, data);
    switch (key) {
        case 'a':
            data -= 100.0;
            if (optional_window)optional_window->set_hovedtrykk(data);
            break;
        case 'b':
            if (optional_window)optional_window->set_bremsetrykk(data);
            break;
        case 'c':
            if (optional_window) optional_window->set_trekkraft(-data);
            the_HID_controller.set_X(-data);
            break;
        case 'd':
            odometer_update(data);
            break;
        case 'x':
//            if (m_WorkerThread[gpc] && m_Worker[gpc].has_stopped()) {
//                if (m_WorkerThread[gpc]->joinable())
//                    m_WorkerThread[gpc]->join();
//                delete m_WorkerThread[gpc];
//                m_WorkerThread[gpc] = nullptr;
//            }
            break;
        default:
            break;
    }
}

void Vognkontroller::odometer_update(double data) {
    auto elapsed = get_elapsed();
    auto speed = (elapsed > 0) ? (data - the_odometer) * 3600.0 / elapsed : 0.0;
    the_odometer = data;
    the_HID_controller.set_Y(speed);
    the_HID_controller.set_Z(the_odometer);
    if (optional_window) {
        optional_window->set_hastighet(speed);
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
