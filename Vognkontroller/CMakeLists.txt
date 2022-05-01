cmake_minimum_required(VERSION 3.16)
project(Vognkontroller)

set(CMAKE_CXX_STANDARD 14)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED IMPORTED_TARGET gtkmm-3.0)
pkg_check_modules(CAIRO REQUIRED IMPORTED_TARGET cairomm-1.0)

set(CMAKE_THREAD_LIBS_INIT "-lpthread")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)

add_executable(Vognkontroller
        src/Vognkontroller.cpp
        src/Vognkontroller.h
        src/My_window.cpp
        src/My_window.h
        src/My_area.cpp
        src/My_area.h
        src/My_worker.cpp
        src/My_worker.h
        src/My_HID_controller.cpp
        src/My_HID_controller.h
        src/hidapi/hidapi.h
        src/hidapi/hid_linux.c
        )

link_directories(/usr/local/include)
link_directories(/usr/local/lib)

include_directories(${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})
target_link_libraries(Vognkontroller ${GTK_LIBRARIES})

include_directories(${CAIRO_INCLUDE_DIRS})
link_directories(${CAIRO_LIBRARY_DIRS})
target_link_libraries(Vognkontroller ${CAIRO_LIBRARIES})

#if (${Platform} STREQUAL Raspberry)
#
    set(MY_LIB_DIR "/usr/lib/arm-linux-gnueabihf")
    target_link_libraries(Vognkontroller
            ${MY_LIB_DIR}/libhidapi-hidraw.so.0
            ${MY_LIB_DIR}/libhidapi-libusb.so.0
            /lib/arm-linux-gnueabihf/libudev.so.1)
#
#elseif (${Platform} STREQUAL Windows)
#
#    set(MY_LIB_DIR "C:/Program Files (x86)/mingw-w64/i686-8.1.0-posix-dwarf-rt_v6-rev0/mingw32/i686-w64-mingw32/lib")
#    add_library(setupapi STATIC IMPORTED)
#    set_target_properties(setupapi PROPERTIES IMPORTED_LOCATION ${MY_LIB_DIR}/libsetupapi.a)
#    target_link_libraries(Vognkontroller setupapi)
#
#else ()
#
#    message(" <> '${Platform}' is not supported <> ")
#
#endif ()
