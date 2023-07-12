cmake_minimum_required(VERSION 3.5)
project(gui)
find_package(SDL2 CONFIG REQUIRED)
FILE(GLOB source "src/*.cpp" "src/*.hpp")
add_executable(gui ${source})
target_link_libraries(gui PRIVATE SDL2::SDL2)
target_compile_features(gui PRIVATE cxx_std_17)

