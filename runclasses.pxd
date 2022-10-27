from libcpp.string cimport string
from libcpp.vector cimport vector

cdef extern from "timestamp.cpp":
    pass

cdef extern from "readers.cpp":
    pass

cdef extern from "runclasses.cpp":
    pass


cdef extern from "runclasses.h":
    cdef cppclass BasicRun:
        BasicRun() except +
        BasicRun(const string& dir) except +
        string getId()  const
        float getTotalTime() const
        float getTotalDist() const
        void print() const

    cdef cppclass StandardRun:
        StandardRun() except +
        StandardRun(const string& dir) except +
        string getId() const
        vector[int] getTimes()
        vector[float] getDistances()
        vector[float] getSpeeds()
        vector[float] getAltitudes()
        vector[float] getLapTimes(int laplength)
        vector[float] getLapDistances(int laplength)
        string getDate() const
        float getTotalTime() const
        float getTotalDistance() const
        float getAvgSpeed() const