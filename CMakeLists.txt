cmake_minimum_required(VERSION 3.20)
project(DBManager VERSION 1.0 LANGUAGES CXX)

# Use modern C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(/utf-8)



# -------------------------------
# Include directories (headers)
# -------------------------------
include_directories(
    Headers
    "Dependencies/glad/include"
    "Dependencies/imgui"
    "Dependencies/imgui/backends"
    "Dependencies/glfw/include"
    "Dependencies/fmt/include"
)

# -------------------------------
# sqlite3
# -------------------------------
add_library(sqlite3 STATIC
    Dependencies/sqlite/sqlite3.c
)
target_include_directories(sqlite3 PUBLIC
    Dependencies/sqlite
)

# -------------------------------
# Source files for your app
# -------------------------------
file(GLOB_RECURSE SOURCES "src/*.cpp")

# -------------------------------
# GLAD
# -------------------------------
add_library(glad STATIC
    Dependencies/glad/src/glad.c
)
target_include_directories(glad PUBLIC
    Dependencies/glad/include
)

# -------------------------------
# ImGui core
# -------------------------------
file(GLOB IMGUI_SOURCES
    Dependencies/imgui/*.cpp
)
add_library(imgui STATIC ${IMGUI_SOURCES})

# ImGui backends (GLFW + OpenGL3)
target_sources(imgui PRIVATE
    Dependencies/imgui/backends/imgui_impl_glfw.cpp
    Dependencies/imgui/backends/imgui_impl_opengl3.cpp
)

# -------------------------------
# GLFW (external project with its own CMake)
# -------------------------------
add_subdirectory(Dependencies/glfw)

# -------------------------------
# Main executable
# -------------------------------
add_executable(DBManager ${SOURCES})

set_target_properties(DBManager PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# -------------------------------
# Find system OpenGL
# -------------------------------
find_package(OpenGL REQUIRED)

# -------------------------------
# Link everything
# -------------------------------
target_link_libraries(DBManager PRIVATE
    imgui
    glad
    glfw
    sqlite3
    OpenGL::GL
)