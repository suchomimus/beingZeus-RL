cmake_minimum_required(VERSION 3.8)
project(bz)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build/")
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build/")

set(SOURCE_FILES
        src/Actor.cpp
        src/Actor.h
        src/Engine.cpp
        src/Engine.h
        src/main.cpp
        src/Map.cpp
        src/Map.h
        include/libtcod.hpp src/BspListener.h src/Tile.h src/Gui.h src/Gui.cpp src/Destructible.cpp src/Destructible.h src/Attacker.cpp src/Attacker.h src/main.h src/Ai.cpp src/Ai.h)

file(GLOB LIBRARIES "${PROJECT_SOURCE_DIR}/lib/*.dylib")
message("LIBRARIES = ${LIBRARIES}")

# add extra include directories
include_directories(${PROJECT_SOURCE_DIR}/include
        /usr/local/include)

# add extra lib directories
# link_directories(${LIBRARIES})

add_executable(bz ${SOURCE_FILES})
target_link_libraries(bz ${LIBRARIES})



