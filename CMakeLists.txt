# Config
cmake_minimum_required(VERSION 3.10)
project(SimpleEngine2D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add subdirectories
add_subdirectory(engine)

# Add all game subdirectories
file(GLOB GAME_DIRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/games" "${CMAKE_CURRENT_SOURCE_DIR}/games/*")
foreach(GAME_DIR ${GAME_DIRS})
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/games/${GAME_DIR}"
       AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/games/${GAME_DIR}/CMakeLists.txt")
        add_subdirectory("games/${GAME_DIR}")
    endif()
endforeach()
