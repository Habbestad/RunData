from libcpp.string cimport string
from libcpp.vector cimport vector
from libcpp cimport bool


cdef extern from "timestamp.cpp":
    pass

cdef extern from "readers.cpp":
    pass

cdef extern from "runclasses.cpp":
    pass


cdef extern from "runclasses.h":
    cdef cppclass Track:
        Track() except +
        Track(int time, float distance, float altitude, float speed, float heartRate) except +

    cdef cppclass RunSummary:
        RunSummary() except +
        RunSummary(const string& dir) except +
        string getId()  const
        float getRunTime() const
        float getRunDist() const
        void print() const

    cdef cppclass Run:
        Run() except +
        Run(const string& dir) except +
        string getId() const
        vector[int] getTimes()
        vector[float] getDistances()
        vector[float] getSpeeds()
        vector[float] getAltitudes()
        vector[float] getHR()
        int getTotalTime()
        float getTotalDistance()
        Run getSectionByTime( int startTime, int endTime, const string& sectionName, bool cumulative)
        Run getSectionByDistance( int start, int end, const string& sectionName, bool cumulative)

