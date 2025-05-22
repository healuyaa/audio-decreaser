cmake_minimum_required(VERSION 3.5.0)
project(audio-decreaser VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/include/*.hpp
    ${CMAKE_SOURCE_DIR}/lib/*.cpp    
)

if(WIN32)
    set(Win_icon ${CMAKE_SOURCE_DIR}/resource/app_icon.rc)
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/stb-image
    ${CMAKE_SOURCE_DIR}/include/GLFW
    ${CMAKE_SOURCE_DIR}/include/libsnd
)

add_executable(audio-decreaser ${SOURCES} ${Win_icon})

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
)

target_link_libraries(audio-decreaser
    PRIVATE ${CMAKE_SOURCE_DIR}/lib/GLFW/glfw3.dll
    PRIVATE glad
    PRIVATE imgui
    PRIVATE ${CMAKE_SOURCE_DIR}/lib/sndfile/sndfile.lib
)