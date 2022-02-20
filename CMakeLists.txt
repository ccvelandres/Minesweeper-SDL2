cmake_minimum_required(VERSION 3.15)
project(Minesweeper VERSION 0.1.0)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

set(CMAKE_C_STANDARD 11) # Requires C11 standard
set(CMAKE_CXX_STANDARD 20) # Requires C++20 standard

file(GLOB_RECURSE SRC_C_CORE src/*.c)
file(GLOB_RECURSE SRC_CPP_CORE src/*.cpp)

message("SRC_CPP_CORE: ${SRC_CPP_CORE}")

add_executable(${PROJECT_NAME} ${SRC_C_CORE} ${SRC_CPP_CORE})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/inc)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2main)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_image)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)


# Checks if OSX and links appropriate frameworks (only required on MacOS)
# if (APPLE)
#     target_link_libraries(${PROJECT_NAME} "-framework IOKit")
#     target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
#     target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
# endif()