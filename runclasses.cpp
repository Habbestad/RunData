#include "readers.h"
#include "runclasses.h"
#include "timestamp.h"

#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <algorithm>


BasicRun::BasicRun() {}

BasicRun::BasicRun(std::ifstream& infile)
: m_id { getData(infile, "Id", "Id") }
{
    std::string time_str;
    std::string dist_str;
    std::string test_str;

    while(test_str != "Done")
    {
        test_str = getData(infile, "Time", "Trackpoint", true);
        if(test_str != "Done")
        {
            time_str = test_str;
            dist_str = getData(infile, "DistanceMeters", "Trackpoint");
        }

    }

    m_totalTime = static_cast<float>(TimeStamp(time_str).secondsPast(TimeStamp(m_id).secondsPast(0)))/60;
    m_totalDist = std::stof(dist_str)/1000;
}

BasicRun::BasicRun(const std::string& str)
{
    std::ifstream infile { str };
    *this = BasicRun(infile);

}

std::string BasicRun::getId()  const { return m_id; }
float BasicRun::getTotalTime() const { return m_totalTime; }
float BasicRun::getTotalDist() const { return m_totalDist; }

void BasicRun::print() const
{
    std::cout << TimeStamp(m_id).getDate() << ", " << m_totalTime << ", " << m_totalDist << '\n';
}

StandardRun::StandardRun(){}

StandardRun::StandardRun(std::ifstream& infile)
{
    std::string id {getData(infile,"Id")};
    m_id = id;
    int initial_time { TimeStamp(m_id).secondsPast(0) };

    std::string time_str;
    bool done { false };
    while(!done)
    {
        time_str = getData(infile, "Time", "Trackpoint", true);
        done = (time_str == "Done");
        if(!done)
        {
            m_times.push_back(TimeStamp(time_str).secondsPast(initial_time));            
            m_distances.push_back(std::stof(getData(infile, "DistanceMeters", "Trackpoint", true)));
            m_speeds.push_back(std::stof(getData(infile, "ns3:Speed", "Trackpoint", true)));
            m_altitudes.push_back(std::stof(getData(infile, "AltitudeMeters", "Trackpoint", false)));   
        }
    }
}


StandardRun::StandardRun(const std::string& str)
{
    std::ifstream infile {str};
    *this = StandardRun(infile);
}

std::string StandardRun::getId() const { return m_id; }
std::string StandardRun::getDate() const { return m_id.substr(0,10); }
float StandardRun::getTotalTime() const { return m_times.back(); }
float StandardRun::getTotalDistance() const { return m_distances.back(); }
float StandardRun::getAvgSpeed() const { return getTotalDistance()/getTotalTime(); }
std::vector<int> StandardRun::getTimes(){ return m_times; }
std::vector<float> StandardRun::getDistances(){ return m_distances; }
std::vector<float> StandardRun::getSpeeds(){ return m_speeds; }
std::vector<float> StandardRun::getAltitudes(){ return m_altitudes; }

std::vector<StandardRun::Lap> StandardRun::getLaps(int laplength)
{
    std::vector<Lap> lapVec;

    std::vector<float>::iterator pos = m_distances.begin();
    std::vector<float>::iterator prev_it;

    int index {0}, prev_index{0};
    float dist {0.0}, time {0.0};
    int correction { 5 };

    while(m_distances.back() >= *pos + laplength)
    {
        prev_index = index;
        prev_it = pos;

        pos = std::find_if(m_distances.begin(),m_distances.end(), [&](float dist){ return dist >= *pos + laplength - correction; } );

        index = pos - m_distances.begin();
        
        dist = *pos - *prev_it;
        time = m_times[index] - m_times[prev_index];

        lapVec.push_back( { dist, time } );
    }

    dist = m_distances.back() - *pos;
    time = m_times.back() - m_times[ index ];

    lapVec.push_back( { dist, time } );

    return lapVec; 
}

std::vector<float> StandardRun::getLapTimes(int k)
{
    std::vector<Lap> laps = getLaps(k);
    std::vector<float> lap_times;

    for(const Lap& lap : laps)
    {
        lap_times.push_back(lap.time);
    }

    return lap_times;
}

std::vector<float> StandardRun::getLapDistances(int k)
{
    std::vector<Lap> laps = getLaps(k);
    std::vector<float> lap_distances;

    for(const Lap& lap : laps)
    {
        lap_distances.push_back(lap.distance);
    }

    return lap_distances;
}





Workout::Workout(std::ifstream& infile) : StandardRun(infile)
{
    infile.clear();
    infile.seekg(0,infile.beg);

    std::string lapTime_str;
    bool done = { false };
    while(!done)
    {
        lapTime_str = getData(infile, "TotalTimeSeconds", "Lap", true);
        done = ("Done" == lapTime_str);
        if(!done)
        {
            m_lapTimes.push_back(std::stof(lapTime_str));
            m_lapDistances.push_back(std::stof(getData(infile, "DistanceMeters", "Lap", false)));
        }

    }
}

std::vector<float> Workout::getLapTimes(){ return m_lapTimes; }
std::vector<float> Workout::getLapDistances(){ return m_lapDistances; }
