cmake_minimum_required(VERSION 3.16)
project(Snake)

set(CMAKE_CXX_STANDARD 14)

add_executable(Snake
#        src/Snake.cpp
        src/RTIMULibDrive11.cpp
        )

link_directories(/usr/local/include)
link_directories(/usr/local/lib)

target_link_libraries(Snake -lRTIMULib)

