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

void My_window::initialize(void *user, void (*func)(void *user, char key, int data)) {
    optional_user = user;
    optional_func = func;
    move(0, 0);
    add(the_box);
    for (uint32_t index = 0; index < My_definitions::Number_of_values; index++) {
        the_area[index].initialize(index);
        the_area[index].set_size_request(300, 50);
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
    for (auto &area: the_area) {
        area.clear();
    }
    auto win = get_window();
    if (win) {
        Gdk::Rectangle r(0, 0, get_allocation().get_width(),
                         get_allocation().get_height());
        win->invalidate_rect(r, false);
    }
    std::cout << "All cleared" << std::endl;
    if (optional_func) {
        optional_func(optional_user, My_definitions::Window_button, 0);
    }
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
    the_area[My_definitions::Hovedtrykk].set_value(data / 100.0);
    the_dispatcher.emit();
}

void My_window::set_bremsetrykk(double data) {
    the_area[My_definitions::Bremsetrykk].set_value(data / 100.0);
    the_dispatcher.emit();
}

void My_window::set_trekkraft(double data) {
    the_area[My_definitions::Trekkraft].set_value(data);
    the_dispatcher.emit();
}

void My_window::set_trippteller(double data) {
    the_area[My_definitions::Trippteller].set_value(data);
    the_dispatcher.emit();
}

void My_window::set_hastighet(double data) {
    the_area[My_definitions::Hastighet].set_value(data);
    the_dispatcher.emit();
}

void My_window::set_omdreininger(double data) {
    the_area[My_definitions::Omdreininger].set_value(data);
    the_dispatcher.emit();
}

void My_window::set_flag(const std::string &text) {
    the_area[My_definitions::Flaggtekst].set_text(text);
    the_dispatcher.emit();
}
