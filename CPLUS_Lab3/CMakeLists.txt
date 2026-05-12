cmake_minimum_required(VERSION 3.20)

project(CPLUS_Lab3 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# Отключаем лишние части GLFW, чтобы собиралась только библиотека.
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)

# GLAD загружает функции OpenGL.
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v2.0.8
    SOURCE_SUBDIR cmake
)

FetchContent_MakeAvailable(glad)

glad_add_library(
    glad_gl_core_33
    STATIC
    API gl:core=3.3
)

# NanoVG не имеет обычного CMake-проекта, поэтому добавляем nanovg.c вручную.
FetchContent_Declare(
    nanovg
    GIT_REPOSITORY https://github.com/memononen/nanovg.git
    GIT_TAG master
)

FetchContent_GetProperties(nanovg)

if(NOT nanovg_POPULATED)
    FetchContent_Populate(nanovg)
endif()

add_library(nanovg STATIC
    "${nanovg_SOURCE_DIR}/src/nanovg.c"
)

target_include_directories(nanovg PUBLIC
    "${nanovg_SOURCE_DIR}/src"
)

# Основная программа.
add_executable(CPLUS_Lab3
    src/main.cpp
    src/NanoVGBackend.cpp

    src/Application.cpp
    src/MenuParser.cpp
    src/MenuSystem.cpp
)

target_include_directories(CPLUS_Lab3 PRIVATE
    "${CMAKE_SOURCE_DIR}/headers"
    "${nanovg_SOURCE_DIR}/src"
)

target_link_libraries(CPLUS_Lab3 PRIVATE
    glfw
    glad_gl_core_33
    nanovg
    opengl32
)

# Копируем папку resources рядом с exe-файлом.
# Это нужно, чтобы программа могла найти resources/menu.gleb при запуске.
add_custom_command(TARGET CPLUS_Lab3 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:CPLUS_Lab3>/resources"
)