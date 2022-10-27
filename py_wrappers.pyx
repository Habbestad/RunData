from libcpp.string cimport string
from libcpp.vector cimport vector

from runclasses cimport BasicRun, StandardRun

cdef class PyBasicRun:
    cdef BasicRun run

    def __init__(self,string dir):
        self.run = BasicRun(dir)

    def getTotalTime(self):
        return self.run.getTotalTime()

    def getTotalDist(self):
        return self.run.getTotalDist()
    
    def print(self):
        return self.run.print()

cdef class PyStandardRun:
    cdef StandardRun run

    def __init__(self,string dir):
        self.run = StandardRun(dir)

    def getTimes(self):
        return self.run.getTimes()

    def getDistances(self):
        return self.run.getDistances()

    def getAltitudes(self):
        return self.run.getAltitudes()

    def getSpeeds(self):
        return self.run.getSpeeds()

    def getLapDistances(self, int laplength = 1000):
        return self.run.getLapDistances(laplength)
    
    def getLapTimes(self, int laplength = 1000):
        return self.run.getLapTimes(laplength)

    def getDate(self):
        return self.run.getDate()

    def getTotalDistance(self):
        return self.run.getTotalDistance()

    def getTotalTime(self):
        return self.run.getTotalTime()


