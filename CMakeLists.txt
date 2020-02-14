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
cmake_minimum_required(VERSION 2.8.12)
# name of the folder
project(life)

add_subdirectory(pybind11)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()
pybind11_add_module(_life ${SOURCES})
target_compile_options(_life PRIVATE ${OpenMP_CXX_FLAGS})
target_link_libraries(_life PRIVATE ${Openmp_CXX_FLAGS})


