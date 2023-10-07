from libcpp.string cimport string
from libcpp.vector cimport vector

from runclasses cimport RunSummary, Run

import numpy as np

cdef class PyLapSummary:
    cdef RunSummary run

    def __init__(self, string dir):
        self.run = RunSummary(dir)

    def getRunTime(self):
        return self.run.getRunTime()

    def getRunDist(self):
        return self.run.getRunDist()
    
    def print(self):
        return self.run.print()


#cdef class PyTrack:
#    cdef Track track
#
#    def __init__(self, time, dist, speed, alt, hr):
#        self.track(time, dist, speed, alt, hr)

cdef class PyRun:
    cdef Run run

    def __init__(self, dir = None):
        if dir == None:
            self.run = Run()
        else:
            self.run = Run(dir)

    def getTimes(self):
        return self.run.getTimes()

    def getDistances(self):
        return self.run.getDistances()

    def getAltitudes(self):
        return self.run.getAltitudes()

    def getSpeeds(self):
        return self.run.getSpeeds()
    
    def getHR(self):
        return self.run.getHR()

    def getTotalTime(self):
        return self.run.getTotalTime()

    def getTotalDistance(self):
        return self.run.getTotalDistance()

    cdef copy(self, Run r):
        self.run = r

    def getSectionByDistance(self, start, end, sectionName, cumulative = False):
        section = PyRun()
        section.copy(self.run.getSectionByDistance(start, end, sectionName, cumulative))
        return section

    def getSectionByTime(self, start, end, sectionName, cumulative = False):
        section = PyRun()
        section.copy(self.run.getSectionByTime(start, end, sectionName, cumulative))
        return section

    def getLaps(self, lapLength):
        lapTimes = []
        lapDistances = []
        totalDist = self.getTotalDistance()
        k = 0
        name = k.to_bytes(2,'big')
        while(k*lapLength <= totalDist):
            k += 1
            lapTimes.append(self.getSectionByDistance((k-1)*lapLength, k*lapLength, name).getTotalTime()  )
            lapDistances.append(self.getSectionByDistance((k-1)*lapLength, k*lapLength, name).getTotalDistance())

        return lapTimes, lapDistances








