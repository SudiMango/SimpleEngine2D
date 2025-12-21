# Find SDL2 packages
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

# Collect all engine source files
file(GLOB_RECURSE ENGINE_SOURCES 
    "src/*.cpp"
)

# Define engine as static library
add_library(SimpleEngine2D STATIC ${ENGINE_SOURCES})

# Specify include directories for the library
target_include_directories(SimpleEngine2D 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link SDL2 to the engine library
target_link_libraries(SimpleEngine2D PUBLIC 
    SDL2::SDL2
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
    SDL2_image::SDL2_image
)
