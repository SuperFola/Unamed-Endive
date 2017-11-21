cmake_minimum_required(VERSION 3.6)
cmake_policy(SET CMP0015 NEW)

project(Unamed)

if(CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -O3")
endif()

file(GLOB_RECURSE PG_UN "${Unamed_SOURCE_DIR}/src/*.hpp")
file(GLOB_RECURSE HD_UN "${Unamed_SOURCE_DIR}/src/*.cpp" "${Unamed_SOURCE_DIR}/main.cpp")

set_property(GLOBAL APPEND_STRING PROPERTY COMPILE_FLAGS -Wall)

include_directories(src/)

find_package(SFML 2 REQUIRED graphics window system network audio)
find_package(PythonLibs 3.6 REQUIRED)

include_directories(${SFML_INCLUDE_DIR})
include_directories(${PYTHON_INCLUDE_DIR})

add_executable(Unamed ${PG_UN} ${HD_UN})

if(SFML_FOUND)
  target_link_libraries(Unamed ${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif()

if (PYTHONLIBS_FOUND)
    target_link_libraries(Unamed ${EXECUTABLE_NAME} ${PYTHON_LIBRARIES})
endif()