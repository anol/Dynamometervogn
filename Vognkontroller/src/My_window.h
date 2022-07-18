//
// Created by aeols on 24.09.2021.
//

#ifndef PINATOR_TEST_MY_WINDOW_H
#define PINATOR_TEST_MY_WINDOW_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <glibmm/dispatcher.h>
#include "My_area.h"

class My_window : public Gtk::Window {
    enum {
        Number_of_values = 6,
    };
    My_area the_area[Number_of_values];
    char the_flag{};
    Gtk::Box the_box;
    Gtk::Button the_button;
    Glib::Dispatcher the_dispatcher{};

public:
    My_window();

    ~My_window() override;

    void initialize();

    void set_hovedtrykk(double data);

    void set_bremsetrykk(double data);

    void set_trekkraft(double data);

    void set_trippteller(double data);

    void set_hastighet(double data);

    void set_flag(const std::string& text);

protected:
    void on_button_clicked();

private:
    void on_notification_from_worker_thread();

    void update_widgets();
};

#endif //PINATOR_TEST_MY_WINDOW_H
