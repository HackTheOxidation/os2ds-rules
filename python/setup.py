from setuptools import Extension, setup

libcprdetector = Extension(name="libcprdetector",
                           language="c++",
                           include_dirs=["include/"],
                           sources=["lib/cpr-detector.cpp"],
                           extra_compile_args=["-O3", "-std=c++20"])

setup(
    ext_modules=[libcprdetector]
    )
