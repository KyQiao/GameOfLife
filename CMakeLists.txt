# default Release
if(NOT CMAKE_CXX_FLAGS_RELEASEKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

file(GLOB SOURCES
    src/*.cpp
    src/*.h
)

set(HOME_INC_DIR "./src") 
include_directories(/src)
cmake_minimum_required(VERSION 2.8.12)
# name of the folder
project(life)

add_subdirectory(pybind11)

# seems in windows only manually set openmp works
if (MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/openmp")
endif(MSVC)
pybind11_add_module(_life ${SOURCES})
target_compile_options(_life PRIVATE ${OpenMP_CXX_FLAGS})
target_link_libraries(_life PRIVATE ${Openmp_CXX_FLAGS})


