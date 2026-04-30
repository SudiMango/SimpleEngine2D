# Find SDL2 package needed by ImGui SDL backends
find_package(SDL2 REQUIRED)

# Build ImGui + backend code as an editor UI library so src/ can include and reuse it
file(GLOB_RECURSE EDITOR_UI_SOURCES CONFIGURE_DEPENDS
    "ui/*.cpp"
)
add_library(SimpleEngine2DEditorUI STATIC ${EDITOR_UI_SOURCES})
target_include_directories(SimpleEngine2DEditorUI
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/ui
        ${CMAKE_CURRENT_SOURCE_DIR}/ui/backend
)
target_link_libraries(SimpleEngine2DEditorUI PUBLIC SDL2::SDL2)

# Build the editor app (src/) and link it to both engine and UI library
file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
)
if(EDITOR_SOURCES)
    add_executable(SimpleEngine2DEditor ${EDITOR_SOURCES})
    target_include_directories(SimpleEngine2DEditor PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/panels
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core
        ${PROJECT_SOURCE_DIR}/engine/include
        ${CMAKE_CURRENT_SOURCE_DIR}/ui
        ${CMAKE_CURRENT_SOURCE_DIR}/ui/backend
    )
    target_link_libraries(SimpleEngine2DEditor PRIVATE
        SimpleEngine2D
        SimpleEngine2DEditorUI
    )
endif()
