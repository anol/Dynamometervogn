cmake_minimum_required(VERSION 3.16)
project(Explore_sense_HAT)

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/Snake.cpp
        src/Frame_buffer.cpp
#        src/RTIMULibDrive11.cpp
        src/main.cpp src/Frame_buffer.cpp src/Frame_buffer.h src/Snake.h)

link_directories(/usr/local/include)
link_directories(/usr/local/lib)
target_link_libraries(${PROJECT_NAME} -lRTIMULib)

