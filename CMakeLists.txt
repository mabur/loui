cmake_minimum_required(VERSION 3.5)
project(loui)
include(FetchContent)
FetchContent_Declare(
        carma
        GIT_REPOSITORY https://github.com/mabur/carma.git
        GIT_TAG 9ae9af87f18a458038fecb4d47ef0677eea1c65e
)
FetchContent_MakeAvailable(carma)
FILE(GLOB source "loui/*.hpp" "loui/*.cpp" "loui/*.h" "loui/*.c")
add_library(loui STATIC ${source})
target_include_directories(loui PUBLIC
        ${carma_SOURCE_DIR}
)
set_target_properties(loui PROPERTIES
        C_STANDARD 23
        C_STANDARD_REQUIRED YES
)
