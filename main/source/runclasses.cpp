#include "tcxparsing.h"
#include "runclasses.h"
#include "timestamp.h"

#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <algorithm>

// --- Implementation of class Run ---

Run::Run(std::ifstream& infile)
{
    m_id  = tcxParsing::getData(infile, "Id");
    int initialTime { TimeStamp(m_id).secondsPast(0) };

    Track track;
    while(tcxParsing::findNext(infile, "Trackpoint") != EOF)
    {
        track.setFromTcxSection(infile, initialTime);
        m_tracks.push_back(track);
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

TrackVecIt Run::findClosestTrackByTime(int measure) const
{
    //TrackVecIt it;
    auto it = std::find_if(m_tracks.begin(), m_tracks.end(), [&](const Track& track)
                                                        { return track.m_time >= measure; });
    if(it == m_tracks.end())
    {
        return it - 1;
    } 
    
    return (it->m_time + (it-1)->m_time > 2*measure) ? it - 1 : it;
}

TrackVecIt Run::findClosestTrackByDistance(int measure) const
{
    //TrackVecIt it;
    // Find the first iterator just beyond the wanted value (the wanted value might not exist)
    auto it = std::find_if(m_tracks.begin(), m_tracks.end(), [&](const Track& track)
                                                        { return track.m_distance >= measure; });

    if(it == m_tracks.end())
    {
        return it - 1;
    } 

    // Check if the last prior iterator is closer than it, and return the closest:
    return (it->m_distance + (it-1)->m_distance > 2*measure) ? it - 1 : it;
}

Run Run::getSection(const TrackVecIt& first, 
                    const TrackVecIt& last, 
                    const std::string& sectionName,
                    bool cumulative) const
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
                    bool cumulative) const
{
    TrackVecIt first = this->findClosestTrackByTime(start);
    TrackVecIt last = this->findClosestTrackByTime(end);

    return getSection(first, last, sectionName, cumulative);
}

Run Run::getSectionByDistance(int start, 
                    int end, 
                    const std::string& sectionName, 
                    bool cumulative) const
{
    TrackVecIt first = this->findClosestTrackByDistance(start);
    TrackVecIt last = this->findClosestTrackByDistance(end);

    return getSection(first, last, sectionName, cumulative);
}

std::string Run::getId() const { return m_id; }

RunSummary Run::getRunSummary() const
{
    Track finalTrack = *(m_tracks.end() - 1);
    RunSummary runSummary(finalTrack.m_time, finalTrack.m_distance, m_id);

    return runSummary;
}

std::vector<int> Run::getTimes() const
{ 
    std::vector<int> times(m_tracks.size());
    std::transform(m_tracks.begin(), m_tracks.end(), times.begin() ,[](const Track& track){ return track.m_time; }  );

    return times;   
}

std::vector<float> Run::getDistances() const
{ 
    std::vector<float> distances(m_tracks.size());
    std::transform(m_tracks.begin(), m_tracks.end(), distances.begin() ,[](const Track& track){ return track.m_distance; }  );

    return distances;  
}

std::vector<float> Run::getSpeeds() const
{ 
    std::vector<float> speeds(m_tracks.size());
    std::transform(m_tracks.begin(), m_tracks.end(), speeds.begin() ,[](const Track& track){ return track.m_speed; }  );

    return speeds;  
}

std::vector<float> Run::getAltitudes() const
{ 
    std::vector<float> altitudes(m_tracks.size());
    std::transform(m_tracks.begin(), m_tracks.end(), altitudes.begin() ,[](const Track& track){ return track.m_altitude; }  );

    return altitudes;
 }

 std::vector<float> Run::getHR() const
 {
    std::vector<float> heartRates(m_tracks.size());
    std::transform(m_tracks.begin(), m_tracks.end(), heartRates.begin() ,[](const Track& track){ return track.m_heartRate; }  ); 

    return heartRates;
 }

int Run::getTotalTime() const { return getTimes().back(); }

float Run::getTotalDistance() const { return getDistances().back(); }


// --- Implementation of class Workout ---

Workout::Workout(std::ifstream& infile) : Run(infile)
{
    infile.clear();
    infile.seekg(0, infile.beg);

    int initial_time { TimeStamp(m_id).secondsPast(0) };
    std::string lapStartTime_str;
    while(tcxParsing::findNext(infile, "Lap StartTime") != EOF)
    {
        lapStartTime_str = tcxParsing::getData(infile, "Time", "Lap StartTime", false);
        m_startTimes.push_back(TimeStamp(lapStartTime_str).secondsPast(initial_time));
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

std::vector<int> Workout::getStartTimes() const
{
    return m_startTimes;
}

Run Workout::getLap(int lapNumber) const
{
    return getSection(m_lapIts.at(lapNumber), m_lapIts.at(lapNumber + 1), std::to_string(lapNumber), false);
}