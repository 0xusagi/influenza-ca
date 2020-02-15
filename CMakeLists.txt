# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("simulation")

set (CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

include_directories (src)

set (src 
        src/config.cpp 
        src/epithelial_cell.cpp 
        src/main.cpp 
        src/utils.cpp 
        src/world.cpp)

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

# Copy dlls
if (WIN32)
    add_custom_command(TARGET simulation POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy_directory 
        "${PROJECT_SOURCE_DIR}/lib/"
        "$<TARGET_FILE_DIR:simulation>")

    add_custom_command(TARGET simulation POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy_directory 
        "${PROJECT_SOURCE_DIR}/images/"
        "$<TARGET_FILE_DIR:simulation>/images/")

    add_custom_command(TARGET simulation POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy_directory 
        "${PROJECT_SOURCE_DIR}/fonts/"
        "$<TARGET_FILE_DIR:simulation>/fonts")
endif()
