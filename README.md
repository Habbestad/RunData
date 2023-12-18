# RunData

### Summary:

RunData is a small c++ library made for extracting data from .tcx files produced by Garmin GPS watches. Using Cython, it can easily be compiled to a Python module, which is how I mainly use it. 

Here is an example of a plot in Python (see 'examples.py' in the 'python' folder for the code):

![bilde](test.png)


### Documentation:

For an overview take a look in the 'main/include' folder. 

To build as a Python 3 module I write

python3 setup.py build_ext --build-lib <build-directory>

in the terminal.

### Motivation

I don't want my watch to notify me every kilometer (a 'lap'), but I often want to look at my average lap paces after the run. However, turning of notifications also turns of the tracking, and so this project was born. I have some plans for more functionality as well, which there are some traces of in the code.








