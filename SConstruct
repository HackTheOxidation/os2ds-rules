include = Dir('include')
env = Environment(CPPPATH=include)
env.MergeFlags(env.ParseFlags("-std=c++20 -O3 -Wall -Wextra -Wshadow -Wconversion -Wpedantic -Werror"))
libcprdetector = env.SharedLibrary('libcprdetector', ['src/cpr-detector.cpp'])