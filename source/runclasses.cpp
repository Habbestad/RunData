#include "readers.h"
#include "runclasses.h"
#include "timestamp.h"

#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <algorithm>

using TrackVecIt = std::vector<Track>::iterator;

// --- Implementation of class RunSummary ---

RunSummary::RunSummary() {}

RunSummary::RunSummary(std::ifstream& infile)
: m_id { getData(infile, "Id", "Id") }
{
    std::string time_str;
    std::string dist_str;
    std::string test_str;

    while(test_str != "Done")
    {
        /* See readers.h and readers.cpp to understand getData() */
        test_str = getData(infile, "Time", "Trackpoint", true);
        if(test_str != "Done")
        {
            time_str = test_str;
            dist_str = getData(infile, "DistanceMeters", "Trackpoint");
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

std::string RunSummary::getId()  const { return m_id; }
float RunSummary::getRunTime() const { return m_runTime; }
float RunSummary::getRunDist() const { return m_runDist; }
float RunSummary::getAverageSpeed() const { return m_runDist/m_runTime; }

void RunSummary::print() const
{
    std::cout << m_id<< ", " << m_runTime << ", " << m_runDist << '\n';
}

// --- Implementation of class Track ---

Track::Track()
: m_time {0}, m_distance {0.0}, m_altitude {0.0}, m_speed {0.0}, m_heartRate {0.0}
{}

Track::Track(int time, float distance, float altitude, float speed, float heartRate)
: m_time {time}, m_distance {distance}, m_altitude {altitude}, m_speed {speed}, m_heartRate {heartRate}
{}

// --- Implementation of class Run ---

Run::Run(){}

Run::Run(std::ifstream& infile)
{
    Track track;

    std::string id {getData(infile,"Id")};
    m_id = id;

    /* TimeStamp is in timestamp.h and timestamp.cpp */
    int initial_time { TimeStamp(m_id).secondsPast(0) };

    std::string time_str;
    bool done { false };
    while(!done)
    {
        /* See readers.h and readers.cpp to understand getData() */
        time_str = getData(infile, "Time", "Trackpoint", true);
        done = (time_str == "Done");
        if(!done)
        {
            track.m_time = TimeStamp(time_str).secondsPast(initial_time);            
            track.m_distance = std::stof(getData(infile, "DistanceMeters", "Trackpoint", true));
            track.m_speed = std::stof(getData(infile, "ns3:Speed", "Trackpoint", true));
            track.m_altitude = std::stof(getData(infile, "AltitudeMeters", "Trackpoint", false));   

            m_tracks.push_back(track);

        }
    }
}

Run::Run(const std::string& str)
{
    std::ifstream infile {str};
    *this = Run(infile);
}

Run::Run(const std::vector<Track>& tracks, const std::string& id)
{
    this->m_id = id;
    this->m_tracks = tracks;
}

TrackVecIt Run::findClosestTrackByTime(int measure)
{
    TrackVecIt it;
    it = std::find_if(m_tracks.begin(), m_tracks.end(), [&](const Track& track)
                                                        { return track.m_time >= measure; });

    if(it == m_tracks.end())
    {
        return it - 1;
    } 
    
    return (it->m_time + (it-1)->m_time > 2*measure) ? it - 1 : it;
}

TrackVecIt Run::findClosestTrackByDistance(int measure)
{
    TrackVecIt it;
    // Find the first iterator just beyond the wanted value (the wanted value might not exist)
    it = std::find_if(m_tracks.begin(), m_tracks.end(), [&](const Track& track)
                                                        { return track.m_distance >= measure; });

    if(it == m_tracks.end())
    {
        return it - 1;
    } 

    // If two neighbouring iterators have the same distance, choose the latter.
    // This is in case there is a tracking gap (big difference in time, but no difference in distance)
    if(it->m_distance == (it+1)->m_distance)
    {
        return it + 1;
    }

    // Check if the last prior iterator is closer than it, and return the closest:
    return (it->m_distance + (it-1)->m_distance > 2*measure) ? it - 1 : it;
}

Run Run::getSection(const TrackVecIt& first, 
                    const TrackVecIt& last, 
                    const std::string& sectionName,
                    bool cumulative)
{
    std::vector<Track> sectionTracks(first,last+1);

    if(!cumulative)
    {
        int startTime = first->m_time;
        float startDistance = first->m_distance;

        auto correction = [&](Track &track) 
        {   
            track.m_time = track.m_time - startTime;
            track.m_distance = track.m_distance - startDistance;

            return track;
        };

        std::transform(sectionTracks.begin(), sectionTracks.end(), sectionTracks.begin() , correction );
    }

    Run section(sectionTracks, m_id + sectionName);
    return section;
}

Run Run::getSectionByTime(int start, 
                    int end, 
                    const std::string& sectionName, 
                    bool cumulative)
{
    TrackVecIt first = this->findClosestTrackByTime(start);
    TrackVecIt last = this->findClosestTrackByTime(end);

    return getSection(first, last, sectionName, cumulative);
}

Run Run::getSectionByDistance(int start, 
                    int end, 
                    const std::string& sectionName, 
                    bool cumulative)
{
    TrackVecIt first = this->findClosestTrackByDistance(start);
    TrackVecIt last = this->findClosestTrackByDistance(end);

    return getSection(first, last, sectionName, cumulative);
}

std::string Run::getId() const { return m_id; }

RunSummary Run::getRunSummary()
{
    Track finalTrack = *(m_tracks.end() - 1);
    RunSummary runSummary(finalTrack.m_time, finalTrack.m_distance, m_id);

    return runSummary;
}

std::vector<int> Run::getTimes()
{ 
    std::vector<int> times(m_tracks.size());
    std::transform(m_tracks.begin(), m_tracks.end(), times.begin() ,[](const Track& track){ return track.m_time; }  );

    return times;   
}

std::vector<float> Run::getDistances()
{ 
    std::vector<float> distances(m_tracks.size());
    std::transform(m_tracks.begin(), m_tracks.end(), distances.begin() ,[](const Track& track){ return track.m_distance; }  );

    return distances;  
}

std::vector<float> Run::getSpeeds()
{ 
    std::vector<float> speeds(m_tracks.size());
    std::transform(m_tracks.begin(), m_tracks.end(), speeds.begin() ,[](const Track& track){ return track.m_speed; }  );

    return speeds;  
}

std::vector<float> Run::getAltitudes()
{ 
    std::vector<float> altitudes(m_tracks.size());
    std::transform(m_tracks.begin(), m_tracks.end(), altitudes.begin() ,[](const Track& track){ return track.m_altitude; }  );

    return altitudes;
 }

 std::vector<float> Run::getHR()
 {
    std::vector<float> heartRates(m_tracks.size());
    std::transform(m_tracks.begin(), m_tracks.end(), heartRates.begin() ,[](const Track& track){ return track.m_heartRate; }  ); 

    return heartRates;
 }

int Run::getTotalTime(){ return getTimes().back(); }

float Run::getTotalDistance(){ return getDistances().back(); }

// --- Implementation of class Workout ---

Workout::Workout(std::ifstream& infile) : Run(infile)
{
    infile.clear();
    infile.seekg(0,infile.beg);

    int initial_time { TimeStamp(m_id).secondsPast(0) };
    std::string lapStartTime_str;
    bool done = { false };
    while(!done)
    {
        lapStartTime_str = getData(infile, "Time", "Lap StartTime", false);
        done = ("Done" == lapStartTime_str);
        if(!done)
        {
            m_startTimes.push_back(TimeStamp(lapStartTime_str).secondsPast(initial_time));
        }
    }

    for(int t : m_startTimes)
    {
        m_lapIts.push_back(std::find_if(m_tracks.begin(), m_tracks.end(), [&](const Track& track){ return track.m_time == t; }));
    }
}

Workout::Workout(const std::string& dir)
{
    std::ifstream infile {dir};
    *this = Workout(infile);
}

std::vector<int> Workout::getStartTimes()
{
    return m_startTimes;
}

Run Workout::getLap(int lapNumber)
{
    return getSection(m_lapIts.at(lapNumber), m_lapIts.at(lapNumber + 1), std::to_string(lapNumber), false);
}