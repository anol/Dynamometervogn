//
// Created by aeols on 24.09.2021.
//

#include "My_window.h"
#include <iostream>

My_window::My_window() :
        the_box(Gtk::ORIENTATION_VERTICAL),
        the_button("Målevogn " __DATE__) {
    set_border_width(20);
    the_button.signal_clicked().connect(sigc::mem_fun(*this, &My_window::on_button_clicked));
    the_dispatcher.connect(sigc::mem_fun(*this, &My_window::on_notification_from_worker_thread));
}

My_window::~My_window() = default;

void My_window::initialize() {
    move(0, 0);
    add(the_box);
    for (uint32_t index = 0; index < Number_of_values; index++) {
        the_area[index].initialize(index);
        the_area[index].set_size_request(400, 100);
        the_box.pack_start(the_area[index]);
    }
    the_box.pack_start(the_button);
    the_button.show();
    for (auto &area: the_area) {
        area.show();
    }
    the_box.show();
}

void My_window::on_button_clicked() {
    the_area[1].set_value(the_area[1].get_value() + 1.1);
    auto win = get_window();
    if (win) {
        Gdk::Rectangle r(0, 0, get_allocation().get_width(),
                         get_allocation().get_height());
        win->invalidate_rect(r, false);
    }
    std::cout << "Hello World" << std::endl;
}

void My_window::on_notification_from_worker_thread() {
    update_widgets();
}

void My_window::update_widgets() {
    auto win = get_window();
    if (win) {
        Gdk::Rectangle r(0, 0, get_allocation().get_width(), get_allocation().get_height());
        win->invalidate_rect(r, false);
    }
}

void My_window::set_hovedtrykk(double data) {
    the_area[0].set_value(data / 100.0);
    the_dispatcher.emit();
}

void My_window::set_bremsetrykk(double data) {
    the_area[1].set_value(data / 100.0);
    the_dispatcher.emit();
}

void My_window::set_trekkraft(double data) {
    the_area[2].set_value(data);
    the_dispatcher.emit();
}

void My_window::set_trippteller(double data) {
    the_area[3].set_value(data);
    the_dispatcher.emit();
}

void My_window::set_hastighet(double data) {
    the_area[4].set_value(data);
    the_dispatcher.emit();
}

void My_window::set_flag(const std::string& text) {
    the_area[5].set_text(text);
    the_dispatcher.emit();
}
