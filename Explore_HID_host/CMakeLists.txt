cmake_minimum_required(VERSION 3.13)
project(cnc_central)

set(CMAKE_CXX_STANDARD 14)

if (${Platform} STREQUAL Windows)

    set(MY_LIB_DIR "C:/Program Files (x86)/mingw-w64/i686-8.1.0-posix-dwarf-rt_v6-rev0/mingw32/i686-w64-mingw32/lib")
    add_library(setupapi STATIC IMPORTED)
    set_target_properties(setupapi PROPERTIES IMPORTED_LOCATION ${MY_LIB_DIR}/libsetupapi.a)

    add_executable(cnc_central
            src/main.cpp
            src/hidapi/hidapi.h
            src/hidapi/hid_windows.c)

    target_link_libraries(cnc_central
            setupapi)

elseif (${Platform} STREQUAL Raspberry)

    set(MY_LIB_DIR "/usr/lib/arm-linux-gnueabihf")

    add_executable(cnc_central
            src/main.cpp
            src/hidapi/hidapi.h
            src/hidapi/hid_linux.c)

    target_link_libraries(cnc_central
            ${MY_LIB_DIR}/libhidapi-hidraw.so.0
            ${MY_LIB_DIR}/libhidapi-libusb.so.0
            /lib/arm-linux-gnueabihf/libudev.so.1)

else ()

    message(" <> '${Platform}' is not supported <> ")

endif ()

