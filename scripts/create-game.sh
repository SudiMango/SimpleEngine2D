#!/bin/bash

set -euo pipefail

if [ $# -ne 1 ]; then
    echo "Usage: $0 <game-name>"
    exit 1
fi

GAME_NAME="$1"

if [[ ! "$GAME_NAME" =~ ^[A-Za-z][A-Za-z0-9_-]*$ ]]; then
    echo "Error: game name must start with a letter and contain only letters, numbers, '-' or '_'."
    exit 1
fi

GAME_DIR="games/$GAME_NAME"

if [ -d "$GAME_DIR" ]; then
    echo "Error: game '$GAME_NAME' already exists at $GAME_DIR."
    exit 1
fi

mkdir -p "$GAME_DIR/src"
mkdir -p "$GAME_DIR"/src/{components,scenes,systems}
mkdir -p "$GAME_DIR"/assets
mkdir -p "$GAME_DIR"/assets/{audio,textures,fonts}

cat > "$GAME_DIR/CMakeLists.txt" <<EOF
# Collect all game source and header files
file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
)
file(GLOB_RECURSE GAME_HEADERS CONFIGURE_DEPENDS
    "src/*.h"
    "src/*.hpp"
)

# Entry point main.cpp
if(NOT EXISTS "\${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
    message(FATAL_ERROR "Expected game entry file: \${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
endif()

# Create the game executable
add_executable(${GAME_NAME} \${GAME_SOURCES} \${GAME_HEADERS})

# Link the game to engine library
target_link_libraries(${GAME_NAME} PRIVATE SimpleEngine2D)

# Include the game's own src directory for its internal headers
target_include_directories(${GAME_NAME} PRIVATE src)
EOF

cat > "$GAME_DIR/src/main.cpp" <<EOF
#include <iostream>

int main(int argc, char *argv[]) {
    std::cout << "Hello from ${GAME_NAME}!" << std::endl;
    return 0;
}
EOF

echo "Created game scaffold at $GAME_DIR"
