#ifndef RUNCLASSES_H
#define RUNCLASSES_H

#include "readers.h"
#include "timestamp.h"

#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <algorithm>

class RunSummary
{
    /* A RunSummary holds an ID, time and distance. It can be initialized from a .tcx file or manually.
       It is also the return type of the Run method getRunSummary(), see below */

private:
    std::string m_id;
    float m_runTime; //{0.0}; //in seconds
    float m_runDist; //{0.0}; //in km

public:
    RunSummary();
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

class Track
{
    /* This class is just an intuitive container for storage of trackpoints (usually collected from a .tcx file). */

private:
    int m_time {0};
    float m_distance {0.0};
    float m_altitude {0.0};
    float m_speed {0.0};
    float m_heartRate {0.0};

public:
    Track();
    Track(int time, float distance, float altitude, float speed, float heartRate = 0.0);

    friend class Run;
    friend class Workout;
};

using TrackVecIt = std::vector<Track>::iterator;

class Run 
{
    /* This is the main class of the project, and is most commonly initialized from reading a .tcx file.
       It holds and ID, "raw tracks", methods for manually extracting sections of the run, and some "getters" */

protected:
    std::string m_id;
    std::vector<Track> m_tracks;

    /* Some convenience functions to find start and end of sections: */
    TrackVecIt findClosestTrackByTime(int measure);         
    TrackVecIt findClosestTrackByDistance(int measure);

    /* Extract a section by providing iterators to m_tracks */
    Run getSection(const TrackVecIt& first, 
                   const TrackVecIt& last, 
                   const std::string& sectionName,
                   bool cumulative = true);

public:
    Run();                          
    Run(std::ifstream& infile);     // Initialize with filestream object
    Run(const std::string& str);    // or with path string
    Run(const std::vector<Track>& tracks, const std::string& id); // or custom

    void operator=(Run run);

    /* Manually extract a section from your run, by specifying start and end points by time or distance.
       Tracks are not evenly distributed, so the functions takess the closest values. As a consequence, a
       a section can be slightly longer or shorter than wanted. */
    Run getSectionByTime( int startTime, int endTime, const std::string& sectionName, bool cumulative = false);
    Run getSectionByDistance(int start, int end, const std::string& sectionName, bool cumulative = false);

    std::string getId() const;
    RunSummary getRunSummary();

    std::vector<int> getTimes();
    std::vector<float> getDistances();
    std::vector<float> getSpeeds();
    std::vector<float> getAltitudes();
    int getTotalTime();
    float getTotalDistance();
};


class Workout : public Run
{
    /* A Workout is a Run which also holds the laps defined in the .tcx file. Any .tcx file contains at least one lap (so 
       strictly speaking any Run can be seen as a Workout). The laps show up in the file according to the settings on the watch
       and/or if the file corresponds to a predefined workout. */

private:
    std::vector<TrackVecIt> m_lapIts;
    std::vector<int> m_startTimes;

public:
    Workout(std::ifstream& infile);
    Workout(const std::string& dir);

    Run getLap(int lapNumber);

    std::vector<int> getStartTimes();

};


#endif


// bool isStop(const TrackVecIt& it, int threshold = 30);
/* Maybe make function that finds big gaps between tracks.
    Use this to not get stupid results for average speed? */ 