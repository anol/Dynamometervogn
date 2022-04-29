cmake_minimum_required(VERSION 3.16)
project(Dynamometerhub)

set(CMAKE_CXX_STANDARD 14)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED IMPORTED_TARGET gtkmm-3.0)
pkg_check_modules(CAIRO REQUIRED IMPORTED_TARGET cairomm-1.0)
#pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET GLIBC_2.4)

set(CMAKE_THREAD_LIBS_INIT "-lpthread")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)

add_executable(Dynamometerhub SRC/main.cpp SRC/My_window.cpp SRC/My_window.h SRC/My_area.cpp SRC/My_area.h SRC/My_worker.cpp SRC/My_worker.h)

link_directories(/usr/local/include)
link_directories(/usr/local/lib)

include_directories(${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})
target_link_libraries(Dynamometerhub  ${GTK_LIBRARIES})

include_directories(${CAIRO_INCLUDE_DIRS})
link_directories(${CAIRO_LIBRARY_DIRS})
target_link_libraries(Dynamometerhub  ${CAIRO_LIBRARIES} )

#target_link_libraries(Dynamometerhub  ${GLIB_LIBRARIES} )


# wx_test.cpp
#target_include_directories(Dynamometerhub PUBLIC "${PROJECT_BINARY_DIR}"
#        /home/pi/Downloads/wxWidgets-3.0.5/include
#        )
#
#link_directories(/home/pi/Downloads/wxWidgets-3.0.5/lib)
