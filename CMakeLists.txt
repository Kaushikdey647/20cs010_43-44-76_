cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")

# Source files directory
set(SRC_DIR lib)

# Source files
file(GLOB SRCS ${SRC_DIR}/*.cpp)

# Object files
foreach(SRC ${SRCS})
    get_filename_component(SRC_NAME ${SRC} NAME_WE)
    set(OBJS ${OBJS} ${SRC_DIR}/${SRC_NAME}.o)
endforeach()

# Target executable
set(TARGET main)

# Rule to build the target executable
add_executable(${TARGET} ${OBJS} main.cpp)

# Include GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add a test executable
add_executable(my_project_tests test1.cpp test2.cpp)  # Add your test source files here

# Link against GoogleTest
target_link_libraries(my_project_tests gtest_main)

# Automatically discover tests
include(GoogleTest)
gtest_discover_tests(my_project_tests)

# Clean rule
add_custom_target(clean COMMAND rm -f ${OBJS} ${TARGET})
