cmake_minimum_required(VERSION 3.26)
project(cpp_predictor)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/resources
        ${PROJECT_BINARY_DIR}/resources
        COMMENT "Copying resources into binary directory")

include_directories(${OPENGL_INCLUDE_DIRS})

# glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
add_subdirectory(glfw)
# glad
add_subdirectory(glad)
# glm
add_subdirectory(glm)

# Dear ImGui
set(IMGUI_DIR imgui/)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)

set(CMAKE_COMPILE_WARNING_AS_ERROR OFF)

include_directories(include)
add_executable(cpp_predictor learning.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        Perceptron.cpp
        Perceptron.h
        gui_main.cpp
        learning.h)

target_link_libraries(${PROJECT_NAME} OpenGL::GL glfw glad glm)
