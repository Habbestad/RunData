#include "runsummary.h"
#include "tcxparsing.h"
#include "timestamp.h"

#include <string>
#include <fstream>
#include <iostream>

RunSummary::RunSummary(std::ifstream& infile)
: m_id { tcxParsing::getData(infile, "Id", "Id") }
{
    std::string time_str;
    std::string dist_str;
    std::string test_str;

    while(test_str != "Done")
    {
        /* See readers.h and readers.cpp to understand getData() */
        test_str = tcxParsing::getData(infile, "Time", "Trackpoint", true);
        if(test_str != "Done")
        {
            time_str = test_str;
            dist_str = tcxParsing::getData(infile, "DistanceMeters", "Trackpoint");
        }

    }

    /* TimeStamp is in timestamp.h and timestamp.cpp */
    m_runTime = static_cast<float>(TimeStamp(time_str).secondsPast(TimeStamp(m_id).secondsPast(0)))/60;
    m_runDist = std::stof(dist_str)/1000;
}

RunSummary::RunSummary(const std::string& str)
{
    std::ifstream infile { str };
    *this = RunSummary(infile);
}

RunSummary::RunSummary(float totalTime, float totalDist, const std::string& lapID)
: m_runTime {totalTime}, m_runDist {totalDist}, m_id {lapID}
{}

void RunSummary::print() const
{
    std::cout << m_id<< ", " << m_runTime << ", " << m_runDist << '\n';
}