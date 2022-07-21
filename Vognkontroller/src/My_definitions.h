//
// Created by aeols on 21.07.2022.
//

#ifndef VOGNKONTROLLER_MY_DEFINITIONS_H
#define VOGNKONTROLLER_MY_DEFINITIONS_H

namespace My_definitions {

    enum {
        Hovedtrykk,
        Bremsetrykk,
        Trekkraft,
        Trippteller,
        Hastighet,
        Omdreininger,
        Flaggtekst,
        Number_of_values
    };

    enum Keys : char {
        Manometer_hovedtrykk  = 'a',
        Manometer_bremsetrykk = 'b',
        Dynamometer_trekkraft = 'c',
        Odometer_key = 'O',
        Odometer_fremover = 'F',
        Odometer_bakover = 'B',
        Button_X = 'X',
        Button_Y = 'Y',
        Button_Z = 'Z',
        Vognkontroller_key = 'V',
        Speedometer = 'S',
        Button_XY = 'y',
        Button_XZ = 'z',
        Window_button = 'W',
        Filelogger_key = 'L',
    };
}
#endif //VOGNKONTROLLER_MY_DEFINITIONS_H
