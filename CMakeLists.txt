cmake_minimum_required(VERSION 3.10)
project(Tetris)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(tetris 
    src/main.cpp
    src/Game.cpp
    src/Board.cpp
    src/Tetromino.cpp
    src/Renderer.cpp
)

target_include_directories(tetris PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(tetris ${SDL2_LIBRARIES})
