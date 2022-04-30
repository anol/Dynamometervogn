//
// Created by aeols on 29.04.2022.
//

#include "Dynamometer.h"

int main(int argc, char *argv[]) {
    Dynamometer hub{};
    return hub.main(argc, argv);
}

int Dynamometer::main(int argc, char *argv[]) {
    auto app = Gtk::Application::create(argc, argv, "no.åøya.dynamometer");
    the_window.initialize();
    initialize_workers();
    return app->run(the_window);
}

void Dynamometer::initialize_workers() {
    for (uint32_t gpc = 0; gpc < Number_of_workers; gpc++) {
        if (!m_WorkerThread[gpc]) {
            m_Worker[gpc].initalize(this, [](void *user, int data) {
                if (user) ((Dynamometer *) user)->notify(data);
            });
            m_WorkerThread[gpc] = new std::thread(&My_worker::run, &m_Worker[gpc]);
        }
    }
}

void Dynamometer::notify(int index) {

}
