//
// Created by aeols on 29.04.2022.
//

#ifndef DYNAMOMETERVOGN_DYNAMOMETER_H
#define DYNAMOMETERVOGN_DYNAMOMETER_H

#include <stdint-gcc.h>

#include "My_window.h"
#include "My_worker.h"

class Dynamometer {
    enum {
        Number_of_workers = 4
    };

    My_window *optional_window{};
    My_worker m_Worker[Number_of_workers] = {
            {0, "/dev/ttyUSB0"},
            {1, "/dev/ttyUSB1"},
            {2, "/dev/ttyUSB2"},
            {3, "/dev/ttyUSB3"}
    };
    std::thread *m_WorkerThread[Number_of_workers]{};

public:
    int main(int argc, char *argv[]);

    void notify(char key, double data);

private:
    My_worker *get_worker(uint32_t index) { return (index < Number_of_workers) ? &m_Worker[index] : nullptr; };

    void initialize_workers();
};


#endif //DYNAMOMETERVOGN_DYNAMOMETER_H
