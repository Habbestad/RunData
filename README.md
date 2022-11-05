# RunData

What: Visualizer of data from my runs using files produced by a Garmin watch. Please note that this is work in progress, so many things
are probably a bit "bloated" and suboptimal. Right now the only somewhat finished thing is to plot data from a single run. It looks like this:

![bilde](test.png)


How: The data is collected (and manipulated a bit) with C++. Python is used for the plotting. Cython is used to combine the two.

Why? Just for fun and to learn. 

Why C++/Python/Cython? I started the project in C++, and I already knew the basics of how to plot in Python using matplotlib. Although there
are probably many nice plotting libraries in C++, I thought it would be cool to learn how to combine C++ and Python anyway. 


The C++ files:

Headers: reader.h, runclasses.h, timestamp.h 

readers.cpp - The Garmin watch produces .tcx files with measurements (time, distance, altitude etc.). These files have a special form,
              and the functions in readers.cpp are made to read these files.

runclasses.cpp - Classes to collect the data:
                  (*) BasicRun only contains total time, total distance and a timestamp (start of the run)
                  (*) StandardRun contains lists of of all the data (times, speeds etc.)
                  (*) Workout is a subclass of StandardRun. Also collects lap data from predefined workouts made on the watch. 

timestamp.cpp - A class for keeping track of timestamps and comparing timestamps.

test.cpp - I just use this to test the things I make. Right now I tested a way to collect several BasicRun's, ultimately to plot a 
           period of runs


Python/Cython files:

setup.py - Setup to build Cython extension.

runclasses.pxd - Like a header file for Cython. Contains declarations of the stuff to be used with Python.

runclasses.pyx - Cython file to wrap the classes in Python.

plot_test.py - First try of plotting a single run. See the picture above.



