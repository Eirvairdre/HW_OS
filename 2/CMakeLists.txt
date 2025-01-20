cmake_minimum_required(VERSION 3.28)
project(ProcLaunch)

set(CMAKE_CXX_STANDARD 17)

add_library(ProcLaunch STATIC test_utility.c
        background_launcher.c)
