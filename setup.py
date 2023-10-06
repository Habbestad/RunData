from setuptools import setup, Extension
from Cython.Build import cythonize

sourcefiles = ["py_wrappers.pyx"]

extensions = Extension("py_wrappers",  sourcefiles, language = "c++", extra_compile_args = ["-std=c++20"])

setup(ext_modules=cythonize(extensions,compiler_directives={'language_level' : "3"}))