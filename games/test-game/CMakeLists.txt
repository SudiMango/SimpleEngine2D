# Collect all game source and header files
file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
)
file(GLOB_RECURSE GAME_HEADERS CONFIGURE_DEPENDS
    "src/*.h"
    "src/*.hpp"
)

# Entry point main.cpp
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
    message(FATAL_ERROR "Expected game entry file: ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
endif()

# Create the game executable
add_executable(test-game ${GAME_SOURCES} ${GAME_HEADERS})

# Link the game to engine library
target_link_libraries(test-game PRIVATE SimpleEngine2D)

# Include the game's own src directory for its internal headers
target_include_directories(test-game PRIVATE src)
