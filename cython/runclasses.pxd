from libcpp.string cimport string
from libcpp.vector cimport vector
from libcpp cimport bool

cdef extern from "timestamp.cpp": pass
cdef extern from "tcxparsing.cpp": pass
cdef extern from "runclasses.cpp": pass
cdef extern from "runsummary.cpp": pass

cdef extern from "runclasses.h":
    cdef cppclass Run:
        Run() except +
        Run(const string& dir) except +
        string getId() const
        vector[int] getTimes() const
        vector[float] getDistances()  const
        vector[float] getSpeeds() const
        vector[float] getAltitudes() const
        vector[float] getHR() const
        int getTotalTime() const
        float getTotalDistance() const
        Run getSectionByTime( int startTime, int endTime, const string& sectionName, bool cumulative) const
        Run getSectionByDistance( int start, int end, const string& sectionName, bool cumulative) const
