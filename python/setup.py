from setuptools import Extension, setup

libcprdetector = Extension(name="cprdetector.detector",
                           language="c++",
                           include_dirs=["../include/"],
                           sources=["src/cprdetector/ffi.cpp", "../lib/cpr-detector.cpp"],
                           extra_compile_args=["-O3", "-std=c++20"])

setup(ext_modules=[libcprdetector])
