cmake_minimum_required(VERSION 3.20)

project(OS2DSRules VERSION 0.0.1)

# Detect available compiler on the host system.
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

# Build Configuration types.
set(CMAKE_CONFIGURATION_TYPES Debug Release)

# Define C++ standard and compiler flags as a target library.
add_library(os2dsrules_compiler_flags INTERFACE)
target_compile_features(os2dsrules_compiler_flags INTERFACE cxx_std_20)
# Build flags.
target_compile_options(os2dsrules_compiler_flags INTERFACE
  "$<${gcc_like_cxx}:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused;-Wconversion;-Wpedantic;-Werror>"
  "$<${msvc_cxx}:/W4:/WX>"						
)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-g -O0 -fsanitize=address,memory,thread,undefined -fsanitize-memory-track-origins")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-O3")


# Compile as a shared library.
add_library(os2dsrules SHARED lib/cpr-detector.cpp lib/address_rule.cpp lib/name_rule.cpp lib/wordlist_rule.cpp)
include_directories(os2dsrules include)
target_link_libraries(os2dsrules PUBLIC os2dsrules_compiler_flags)


# Compile test suite.
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
## CPRRule
add_executable(testcpr tests/testcpr.cpp)
target_include_directories(testcpr PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR/include}")
target_link_libraries(testcpr ${GTEST_LIBRARIES} os2dsrules os2dsrules_compiler_flags)
## Data Structures
add_executable(testds tests/testds.cpp)
target_include_directories(testds PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR/include}")
target_link_libraries(testds ${GTEST_LIBRARIES} os2dsrules os2dsrules_compiler_flags)
## NameRule
add_executable(testname tests/testname.cpp)
target_include_directories(testname PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR/include}")
target_link_libraries(testname ${GTEST_LIBRARIES} os2dsrules os2dsrules_compiler_flags)
## AddressRule
add_executable(testaddress tests/testaddress.cpp)
target_include_directories(testaddress PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR/include}")
target_link_libraries(testaddress ${GTEST_LIBRARIES} os2dsrules os2dsrules_compiler_flags)

add_test(cpr_unittests testcpr)
add_test(datastructures_unittests testds)
add_test(name_unittests testname)
add_test(address_unittests testaddress)


# Install library on system.
set(installable_libs os2dsrules os2dsrules_compiler_flags)
install(TARGETS ${installable_libs} DESTINATION lib)
# Install header files on system.
install(FILES include/os2dsrules.hpp include/cpr-detector.hpp include/data_structures.hpp include/name_rule.hpp include/wordlist_rule.hpp DESTINATION include)
# CPack for making package distributions.
include(CPack)
