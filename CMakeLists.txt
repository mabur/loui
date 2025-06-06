cmake_minimum_required(VERSION 3.5)
project(loui)
FILE(GLOB source "loui/*.hpp" "loui/*.cpp" "loui/*.h" "loui/*.c")
add_library(loui STATIC ${source})
set_target_properties(loui PROPERTIES
        C_STANDARD 23
        C_STANDARD_REQUIRED YES
)
