cmake_minimum_required(VERSION 3.12)

project(RAYCASTER  VERSION 1.0.0
        DESCRIPTION "Raycasting Engine"
        LANGUAGES CXX)


# Usage:
# cd build
# cmake .. -G "MinGW Makefiles"
# make

set(DEBUG TRUE)

link_directories(lib)
set(SDL2_DIR "lib/cmake/SDL2")
include_directories(include)
include_directories(include/headers)
include_directories(include/SDL2)

############### Main Build ###############

file(GLOB RAYCAST_SRC CONFIGURE_DEPENDS "src/*.cpp")

add_executable(RAYCASTER ${RAYCAST_SRC})

find_package(SDL2 REQUIRED)

target_link_libraries(RAYCASTER mingw32)
target_link_libraries(RAYCASTER SDL2main)
target_link_libraries(RAYCASTER SDL2_ttf)
target_link_libraries(RAYCASTER SDL2)

if(DEBUG)
    set(CMAKE_CXX_FLAGS "-Wall -DDEBUG")
else()
    set(CMAKE_CXX_FLAGS "-Wl,-subsystem,windows -O3")  
endif()