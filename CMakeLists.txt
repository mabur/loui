cmake_minimum_required(VERSION 3.5)
project(gui90)
FILE(GLOB source "gui90/*.hpp" "gui90/*.cpp" "gui90/*.h" "gui90/*.c")
add_library(gui90 STATIC ${source})
