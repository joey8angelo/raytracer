cmake_minimum_required(VERSION 3.10)
project(Raytracer)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(PNG REQUIRED)
find_package(Curses REQUIRED)
find_package(OpenMP REQUIRED)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(INCLUDE_DIR ${SOURCE_DIR})

# Source files
file(GLOB_RECURSE SRC_FILES 
    "${SOURCE_DIR}/*.cpp"
    "${SOURCE_DIR}/lights/*.cpp"
    "${SOURCE_DIR}/objects/*.cpp"
    "${SOURCE_DIR}/shaders/*.cpp"
)

# Release configuration
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${PNG_LIBRARIES} ${CURSES_LIBRARIES} ${OpenMP_CXX_LIBRARIES})
target_compile_options(${PROJECT_NAME} PRIVATE -O3 -Wall -Werror -fopenmp -DNDEBUG)

# Debug configuration
add_executable(${PROJECT_NAME}_debug ${SRC_FILES})
target_include_directories(${PROJECT_NAME}_debug PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME}_debug PRIVATE ${PNG_LIBRARIES} ${CURSES_LIBRARIES} ${OpenMP_CXX_LIBRARIES})
target_compile_options(${PROJECT_NAME}_debug PRIVATE -g -O0 -fopenmp)
