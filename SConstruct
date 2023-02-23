include = Dir('include')
env = Environment(CPPPATH=include)
env.MergeFlags(env.ParseFlags("-std=c++20 -O3 -Wall -Wextra -Wshadow -Wconversion -Wpedantic -Werror"))

libcprdetector = env.SharedLibrary('libcprdetector', ['lib/cpr-detector.cpp'])
testcprdetector = env.Program('test', ['lib/cpr-detector.cpp', 'tests/test.cpp'])

env.Install('/usr/lib', libcprdetector)
env.Alias('install', '/usr/lib')

Mkdir('/usr/include/libcprdetector')
env.Install('/usr/include/libcprdetector', ['include/cpr-detector.hpp'])
env.Alias('install', '/usr/include/libcprdetector')
