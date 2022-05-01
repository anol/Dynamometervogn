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
        return app->run(*optional_window);
    } else {
        return -2;
    }
}

void Vognkontroller::initialize_workers() {
    for (uint32_t gpc = 0; gpc < Number_of_workers; gpc++) {
        if (!m_WorkerThread[gpc]) {
            m_Worker[gpc].initalize(this, [](void *user, char key, double data) {
                if (user) ((Vognkontroller *) user)->notify(key, data);
            });
            m_WorkerThread[gpc] = new std::thread(&My_worker::run, &m_Worker[gpc]);
        }
    }
}

void Vognkontroller::notify(char key, double data) {
    switch (key) {
        case 'a':
            if (optional_window)optional_window->set_hovedtrykk(data);
            break;
        case 'b':
            if (optional_window)optional_window->set_bremsetrykk(data);
            break;
        case 'c':
            if (optional_window) optional_window->set_trekkraft(data);
            break;
        case 'd':
            if (optional_window) optional_window->set_trippteller(data);
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