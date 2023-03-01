from setuptools import Extension, setup

CXX_FLAGS = (
    "-O3",
    "-std=c++20",
    )

CPR_SOURCES = (
    "src/os2ds_rules/cpr.cpp",
    "lib/cpr-detector.cpp",
    )

cpr_detector = Extension(name="os2ds_rules.cpr_detector",
                           language="c++",
                           include_dirs=["include/"],
                           sources=[*CPR_SOURCES],
                           extra_compile_args=[*CXX_FLAGS])

setup(
    ext_modules=[
        cpr_detector,
      ]
    )
