cmake_minimum_required(VERSION 3.29)
project(Lecture_Scheduler)

set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")

add_executable(Lecture_Scheduler
        main.cpp
)
