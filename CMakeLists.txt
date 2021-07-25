cmake_minimum_required(VERSION 3.10)
# name of the folder
project(life)

option(BINARY "OpenGL example" ON)


# default Release
if(NOT CMAKE_CXX_FLAGS_RELEASEKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(SOURCES "src/life.cpp")

set(PythonSOURCES "src/life.cpp" "src/python.cpp")


set(HOME_INC_DIR "./src")
include_directories(/src)

add_subdirectory(pybind11)

find_package(OpenMP)
pybind11_add_module(_life ${PythonSOURCES})
target_link_libraries(_life PRIVATE OpenMP::OpenMP_CXX)

add_library(life ${SOURCES})


IF(BINARY)
find_package(OpenGL REQUIRED)
add_executable(GameOfLife src/main.cpp)
include_directories(include)
target_link_libraries(GameOfLife life "${OPENGL_LIBRARIES}"
                                        "${CMAKE_SOURCE_DIR}/glewd.lib" 
                                        "${CMAKE_SOURCE_DIR}/glfw3.lib" 
                                        "OpenMP::OpenMP_CXX")
endif(BINARY)