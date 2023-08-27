cmake_minimum_required(VERSION 3.12)


project(cspiro CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(externals)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(cspiro
    "src/main.cpp"

    "src/spirograph/spiro_node.hpp"
    "src/spirograph/circle_transform.hpp"

    "src/graphics/line_strip.hpp"
    "src/graphics/camera.hpp"
    "src/graphics/vertex_2d.hpp"
    "src/graphics/shader_program.hpp"
    "src/graphics/shader.hpp"
    
    "src/application/application.hpp"
    
    
    "src/spirograph/spiro_node.cpp"
    "src/spirograph/circle_transform.cpp"

    "src/graphics/line_strip.cpp"
    "src/graphics/camera.cpp"
    "src/graphics/shader_program.cpp"
    "src/graphics/shader.cpp"

    "src/application/application.cpp"
)

target_include_directories(cspiro PUBLIC include)
target_include_directories(cspiro PRIVATE src)

target_link_libraries(cspiro glm SDL2 libglew_shared)