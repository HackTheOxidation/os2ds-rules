include = Dir('include')
env = Environment(CPPPATH=include)
env.MergeFlags(env.ParseFlags("-std=c++20 -O3 -Wall -Wextra -Wshadow -Wconversion -Wpedantic -Werror"))

libos2dsrules = env.SharedLibrary('libos2dsrules', ['lib/cpr-detector.cpp'])
testos2dsrules = env.Program('test', ['lib/cpr-detector.cpp', 'tests/test.cpp'])

env.Install('/usr/lib', libos2dsrules)
env.Alias('install', '/usr/lib')

Mkdir('/usr/include/libos2dsrules')
env.Install('/usr/include/libos2dsrules', ['include/cpr-detector.hpp'])
env.Alias('install', '/usr/include/libos2dsrules')
