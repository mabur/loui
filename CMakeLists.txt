cmake_minimum_required(VERSION 3.5)
project(gui90)
FILE(GLOB source "gui90/*.hpp" "gui90/*.cpp")
add_library(gui90 ${source})
