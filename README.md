# RunData

### Summary:

RunData is a small c++ library made for extracting data from .tcx files produced by my Garmin GPS watch. Using Cython, it can easily be compiled to a Python module, which is how I mainly use it. 

Here is an example of a plot in Python (see 'examples.py' in the 'python' folder for the code):

![bilde](bilde.png)


### Documentation:

For an overview take a look in the 'main/include' folder. 

### Motivation

I don't want my watch to notify when I have run a 'lap' by vibrating every kilometer. Still, I often want to look at my average lap paces after the run. However, if I turn of the  notifications the watch stops tracking as well. This is the main reason I started this project. I have some plans for more functionality as well, which there are some traces of in the code.

### Disclainer

This is very much a hobby project in developement. There is no error or exception handling. Also, for learning purposes, some parts are unnecessesarily (and probably sub-optimally) made from scratch.








