#    Lecture-Scheduler - A program made for showing info about lectures.
#    Copyright (C) 2025 Emirhan Kotan
#    Original work: https://github.com/Authlantech/Lecture-Scheduler
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License version 3 as published by
#    the Free Software Foundation.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License version 3 for more details.
#
#    You should have received a copy of the GNU General Public License version 3
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.


cmake_minimum_required(VERSION 3.29)
project(Lecture_Scheduler)

set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")

add_executable(Lecture-Scheduler
        main.cpp
)
