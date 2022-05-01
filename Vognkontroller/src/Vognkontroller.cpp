//
// Created by aeols on 29.04.2022.
//

#include "Vognkontroller.h"

int main(int argc, char *argv[]) {
    Vognkontroller hub{};
    return hub.main(argc, argv);
}

int Vognkontroller::main(int argc, char *argv[]) {
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
    switch (key) {
        case 'a':
            data -= 100.0;
            if (optional_window)optional_window->set_hovedtrykk(data);
            the_HID_controller.set_X(data / 100.0);
            break;
        case 'b':
            if (optional_window)optional_window->set_bremsetrykk(data);
            break;
        case 'c':
            if (optional_window) optional_window->set_trekkraft(-data);
            the_HID_controller.set_Y(-data);
            break;
        case 'd':
            if (optional_window) optional_window->set_trippteller(data);
            the_HID_controller.set_Z(data);
            break;
        case 'e':
            if (optional_window) optional_window->set_hastighet(data);
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