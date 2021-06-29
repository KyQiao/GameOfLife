cmake_minimum_required(VERSION 3.10)
# name of the folder
project(life)


# default Release
if(NOT CMAKE_CXX_FLAGS_RELEASEKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

file(GLOB SOURCES
    src/life.cpp
    src/python.cpp
    src/*.h
    )

set(HOME_INC_DIR "./src") 
include_directories(/src)

add_subdirectory(pybind11)


pybind11_add_module(_life ${SOURCES})
target_link_libraries(_life PRIVATE ${Openmp_CXX_FLAGS})


