//
// Created by aeols on 24.09.2021.
//

#include "My_area.h"
#include "My_window.h"

My_area::My_area() = default;

My_area::~My_area() = default;

bool My_area::on_draw(const Cairo::RefPtr<Cairo::Context> &cr) {
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();
    // Draw a black rectangle
    cr->set_source_rgb(0.0, 0.0, 0.0);
    draw_rectangle(cr, width, height);
    // and some white text
    enum {
        Buffer_size = 16
    };
    char buffer[Buffer_size];
    switch (the_index) {
        case My_definitions::Hovedtrykk:
            sprintf(buffer, "%.2f bar", the_value);
            break;
        case My_definitions::Bremsetrykk:
            sprintf(buffer, "%.2f bar", the_value);
            break;
        case My_definitions::Trekkraft:
            sprintf(buffer, "%.2f kg ", the_value);
            break;
        case My_definitions::Trippteller:
            sprintf(buffer, "%.2f m  ", the_value);
            break;
        case My_definitions::Hastighet:
            sprintf(buffer, "%.2f km/t", the_value);
            break;
        case My_definitions::Omdreininger:
            sprintf(buffer, "%.2f cyc", the_value);
            break;
        default:
            strncpy(buffer, the_text.c_str(), Buffer_size);
            break;
    }
    cr->set_source_rgb(1.0, 1.0, 1.0);
    draw_text(buffer, cr, width, height);
    return true;
}

void My_area::draw_rectangle(const Cairo::RefPtr<Cairo::Context> &cr,
                             int width, int height) {
    cr->rectangle(0, 0, width, height);
    cr->fill();
}

void My_area::draw_text(const char *text, const Cairo::RefPtr<Cairo::Context> &cr,
                        int rectangle_width, int rectangle_height) {
    // http://developer.gnome.org/pangomm/unstable/classPango_1_1FontDescription.html
    Pango::FontDescription font;
    font.set_family("Monospace");
    font.set_weight(Pango::WEIGHT_BOLD);
    font.set_size(24 * PANGO_SCALE);
    // http://developer.gnome.org/pangomm/unstable/classPango_1_1Layout.html
    auto layout = create_pango_layout(text);
    layout->set_font_description(font);
    int text_width;
    int text_height;
    //get the text dimensions (it updates the variables -- by reference)
    layout->get_pixel_size(text_width, text_height);
    // Position the text in the middle
    cr->move_to((rectangle_width - text_width) / 2, (rectangle_height - text_height) / 2);
    layout->show_in_cairo_context(cr);
}
