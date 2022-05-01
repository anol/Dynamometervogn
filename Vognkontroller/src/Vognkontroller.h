//
// Created by aeols on 29.04.2022.
//

#ifndef DYNAMOMETERVOGN_DYNAMOMETER_H
#define DYNAMOMETERVOGN_DYNAMOMETER_H

#include <stdint-gcc.h>

#include "My_window.h"
#include "My_worker.h"
#include "My_filelogger.h"
#include "My_HID_controller.h"

class Vognkontroller {
    enum {
        Number_of_workers = 4
    };

    My_filelogger the_logger{};
    My_window *optional_window{};
    My_worker m_Worker[Number_of_workers] = {
            {0, "/dev/ttyUSB0"},
            {1, "/dev/ttyUSB1"},
            {2, "/dev/ttyUSB2"},
            {3, "/dev/ttyUSB3"}
    };
    std::thread *optional_worker_thread[Number_of_workers]{};
    My_HID_controller the_HID_controller{};
    std::thread *optional_HID_thread{};

public:
    int main(int argc, char *argv[]);

    void notify(char key, double data);

private:
    My_worker *get_worker(uint32_t index) { return (index < Number_of_workers) ? &m_Worker[index] : nullptr; };

    void initialize_workers();

    void initialize_HID_controller();
};


#endif //DYNAMOMETERVOGN_DYNAMOMETER_H
