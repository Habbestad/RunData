from setuptools import setup, Extension
from Cython.Build import cythonize

sourcefiles = ["pyRunData.pyx"]

extensions = Extension("pyRunData",  
                       sourcefiles, 
                       language = "c++", 
                       extra_compile_args = ["-std=c++20"],
                       include_dirs = ['../main/source', '../main/include'])

setup(name = 'pyRunData', ext_modules=cythonize(extensions,compiler_directives={'language_level' : "3"}))

# To build: python3 setup.py build_ext --build-lib build