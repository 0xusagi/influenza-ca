# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("simulation")

set (CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

include_directories (
    src 
    third_party/lodepng
)

if ( CMAKE_COMPILER_IS_GNUCC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()
if ( MSVC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")
endif()

set (src 
        src/config.cpp 
        src/epithelial_cell.cpp 
        src/immune_cell.cpp 
        src/input.cpp 
        src/graphics.cpp 
        src/main.cpp 
        src/utils.cpp 
        src/viewport_info.cpp 
        src/viewport_grid.cpp 
        src/window.cpp 
        src/world.cpp)

set (png_src
        third_party/lodepng/lodepng.cpp)

add_executable(simulation ${src})

# SDL libraries
if (DEFINED SDL2_INCLUDE_DIRS AND DEFINED SDL2_LIBRARIES)
	message(STATUS "Using pre-defined SDL2 variables SDL2_INCLUDE_DIRS and SDL2_LIBRARIES")
	target_include_directories(simulation PUBLIC "$<BUILD_INTERFACE:${SDL2_INCLUDE_DIRS}>")
	target_link_libraries(simulation ${SDL2_LIBRARIES})
else()
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_ttf REQUIRED)

    include_directories(${SDL2_INCLUDE_DIR}
                        ${SDL2_IMAGE_INCLUDE_DIR}
                        ${SDL2_TTF_INCLUDE_DIR})
    target_link_libraries(simulation ${SDL2_LIBRARY}
                                ${SDL2_IMAGE_LIBRARIES}
                                ${SDL2_TTF_LIBRARIES}) 
endif()

