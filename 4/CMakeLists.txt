cmake_minimum_required(VERSION 3.10)
project(temperature_monitor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(main
        src/main.cpp
        src/serial_port.cpp
        src/logger.cpp
        src/statistics.cpp
        src/signal_handler.cpp
)

add_executable(sim
        src/sim.cpp
        src/serial_port.cpp
        src/logger.cpp
        src/signal_handler.cpp
)