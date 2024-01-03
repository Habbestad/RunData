# RunData

### Summary:

RunData is a small c++ library made for extracting data from the .tcx files produced by my Garmin GPS watch. Using Cython, it can easily be compiled to a Python module, which is how I mainly use it. 

### Examples

The library is not very interesting without some visualisation tools. A small example using Matplot++ can be found in main/testplot.
However, I have not found any C++ libraries that are as flexible and easy to use as Matplotlib (Python). Here are some examples made using Matplotlib by exporting (parts of) RunData as a Python module:

![bilde](bilde.png)

An interactive example:

![](spanselector.gif)

The code for these are in the 'python' folder.

### Documentation:

For an overview take a look in the 'main/include' folder. 

### Disclaimer

This is very much a hobby project in developement. 







