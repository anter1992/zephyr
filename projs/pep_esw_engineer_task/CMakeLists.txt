# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(pep_esw_engineer_task)
include_directories(${CMAKE_SOURCE_DIR}/inc)
target_sources(app PRIVATE src/main.cpp
                src/ReadClass.cpp 
                src/ReactClass.cpp)
