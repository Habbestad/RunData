#ifndef RUNSUMMARY_H
#define RUNSUMMARY_H

#include <string>
#include <fstream>
#include <iostream>

#include "tcxparsing.h"
#include "timestamp.h"

class RunSummary
{
    /* A RunSummary holds an ID, time and distance. It can be initialized from a .tcx file or manually.
       It is also the return type of the Run method getRunSummary(), see below */
private:
    std::string m_id;
    float m_runTime; //{0.0}; //in seconds
    float m_runDist; //{0.0}; //in km

public:
    RunSummary() { }
    RunSummary(std::ifstream& infile);    // Initialize with filestream object,
    RunSummary(const std::string& str);   // or with path string,
    RunSummary(float totalTime, float totalDist, const std::string& lapID); // or manually.

    std::string getId()  const;
    float getRunTime() const;
    float getRunDist() const;
    float getAverageSpeed() const;

    void print() const;

    friend class Run;
};

#endif