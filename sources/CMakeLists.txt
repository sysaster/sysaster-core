cmake_minimum_required(VERSION 3.13)

project(sysaster VERSION 0.1.0 LANGUAGES CXX)

include(FetchContent)

# deps
#-----------------------------------------#

find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED)

# define build types and configurations
# --------------------------------------- #
if (CMAKE_CONFIGURATION_TYPES)
    if (NOT "Test" IN_LIST CMAKE_CONFIGURATION_TYPES)
        list(APPEND CMAKE_CONFIGURATION_TYPES Test)
    endif()
else()
    set(allowableBuildTypes Debug Test Release)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "${allowableBuildTypes}")
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
    elseif(NOT CMAKE_BUILD_TYPE IN_LIST allowableBuildTypes)
        message(FATAL_ERROR "Invalid build type ${CMAKE_BUILD_TYPE}") 
    endif()
endif()

set(CMAKE_TEST_POSTFIX _tests)

# use c++17
# --------------------------------------- #
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(PYTHON_LIBRARY /usr/lib/libpython2.7.so) 
set(PYTHON_INCLUDE_DIR /usr/include/python2.7)
set(PYTHON_EXECUTABLE /usr/bin/python2.7)

find_package(PythonLibs 2.7 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

# submodules
# --------------------------------------- #
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

# libraries
# --------------------------------------- #
add_library(cppbase64 src/extern/base64.cpp)
target_include_directories(cppbase64 PUBLIC include)

# executables
# --------------------------------------- #
add_executable(sysaster src/SysasterMain.cpp)
target_link_libraries(sysaster PUBLIC ${OpenCV_LIBS} PUBLIC restclient-cpp PUBLIC curl PRIVATE cppbase64 PRIVATE ${PYTHON_LIBRARIES})
target_include_directories(sysaster PUBLIC include PUBLIC ${OpenCV_INCLUDE_DIRS} PRIVATE ${Python2_NumPy_INCLUDE_DIRS})

# test definitions
# use googletest framework
# --------------------------------------- #
FetchContent_Declare(googletest
    GIT_REPOSITORY  https://github.com/google/googletest.git
    GIT_TAG         2fe3bd994b3189899d93f1d5a881e725e046fdc2 # 1.8.1
)
FetchContent_GetProperties(googletest)
if (NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

include(GoogleTest)

set(test_sources ${test_sources}
    tests/main.cpp
    tests/tello_tests.cpp
)

add_executable(sysastercoretests ${test_sources})
target_link_libraries(sysastercoretests PRIVATE gtest PUBLIC ${OpenCV_LIBS} PRIVATE ${PYTHON_LIBRARIES})
target_include_directories(sysastercoretests PRIVATE include PUBLIC ${OpenCV_INCLUDE_DIRS} PRIVATE ${Python2_NumPy_INCLUDE_DIRS})

gtest_discover_tests(sysastercoretests)

enable_testing()
