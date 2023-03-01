cmake_minimum_required(VERSION 3.10)

project(OS2DSRules VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(os2dsrules SHARED lib/cpr-detector.cpp)
include_directories(os2dsrules include)

add_executable(test tests/test.cpp)
target_include_directories(test PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR/include}")
target_link_libraries(test os2dsrules)


