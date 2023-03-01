cmake_minimum_required(VERSION 3.20)

project(OS2DSRules VERSION 0.0.1)

# Detect available compiler on the host system.
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

# Define C++ standard and compiler flags as a target library.
add_library(os2dsrules_compiler_flags INTERFACE)
target_compile_features(os2dsrules_compiler_flags INTERFACE cxx_std_20)
# Build flags.
target_compile_options(os2dsrules_compiler_flags INTERFACE
  "$<${gcc_like_cxx}:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused;-Wconversion;-Wpedantic;-Werror;-O3>"
  "$<${msvc_cxx}:-W3>"						
)

# Compile as a shared library.
add_library(os2dsrules SHARED lib/cpr-detector.cpp)
include_directories(os2dsrules include)
target_link_libraries(os2dsrules PUBLIC os2dsrules_compiler_flags)

# Compile test suite.
enable_testing()
add_executable(testsuite tests/test.cpp)
target_include_directories(testsuite PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR/include}")
target_link_libraries(testsuite os2dsrules os2dsrules_compiler_flags)
add_test(unittests testsuite)

# Install library on system.
set(installable_libs os2dsrules os2dsrules_compiler_flags)
install(TARGETS ${installable_libs} DESTINATION lib)

# Install header files on system.
install(FILES include/os2dsrules.hpp include/cpr-detector.hpp DESTINATION include)
