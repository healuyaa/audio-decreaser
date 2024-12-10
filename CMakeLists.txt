cmake_minimum_required(VERSION 3.5.0)
project(audio-decreaser VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/include/*.hpp
    ${CMAKE_SOURCE_DIR}/lib/*.cpp
)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/stb-image
    ${CMAKE_SOURCE_DIR}/include/minaudio
    ${GTK3_INCLUDE_DIRS}
    ${wxWidgets_USE_FILE}
)

find_package(glfw3 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

add_executable(audio-decreaser ${SOURCES})

add_library(glad ${CMAKE_SOURCE_DIR}/lib/glad/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/lib/glad)

add_library(imgui
    ${CMAKE_SOURCE_DIR}/lib/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui
    PUBLIC ${CMAKE_SOURCE_DIR}/include/imgui
    PUBLIC ${CMAKE_SOURCE_DIR}/include/imgui/backends
    PUBLIC ${GTK3_INCLUDE_DIRS}
)

link_directories(/usr/lib/x86_64-linux-gnu)
target_link_libraries(audio-decreaser
    PRIVATE glad
    PRIVATE glfw
    PRIVATE imgui
    PRIVATE ${GTK3_LIBRARIES}
    PRIVATE ${wxWidgets_LIBRARIES}
)