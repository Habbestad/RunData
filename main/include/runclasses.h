#ifndef RUNCLASSES_H
#define RUNCLASSES_H

#include "tcxparsing.h"
#include "timestamp.h"
#include "track.h"
#include "runsummary.h"

#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include <algorithm>

using TrackVecIt = std::vector<Track>::iterator;

class Run 
{
    /* This is the main class of the project, and is most commonly initialized from reading a .tcx file.
       It holds an ID, "raw tracks", methods for manually extracting sections of the run, and some "getters" */
public:
    Run(){};                          
    Run(std::ifstream& infile);     // Initialize with filestream object
    Run(const std::string& str);    // or with path string
    Run(const std::vector<Track>& tracks, const std::string& id); // or custom

    /* Manually extract a section from your run, by specifying start and end points by time or distance.
       Tracks are not evenly distributed, so the functions takess the closest values. As a consequence, a
       a section can be slightly longer or shorter than wanted. */
    Run getSectionByTime(int startTime, 
                        int endTime, 
                        const std::string& sectionName, 
                        bool cumulative = false);

    Run getSectionByDistance(int start, 
                            int end, 
                            const std::string& sectionName, 
                            bool cumulative = false);

    // Getters:
    std::string getId() const;
    RunSummary getRunSummary();

    std::vector<int> getTimes();
    std::vector<float> getDistances();
    std::vector<float> getSpeeds();
    std::vector<float> getAltitudes();
    std::vector<float> getHR();
    int getTotalTime();
    float getTotalDistance();


protected:
    std::string m_id;
    std::vector<Track> m_tracks;

    /* Extract a section by providing iterators to m_tracks */
    Run getSection(const TrackVecIt& first, 
                const TrackVecIt& last, 
                const std::string& sectionName,
                bool cumulative = true);


private:
    /* Some convenience functions to find start and end of sections: */
    TrackVecIt findClosestTrackByTime(int measure);         
    TrackVecIt findClosestTrackByDistance(int measure);
};


class Workout : public Run
{
    /* A Workout is a Run which also holds the laps defined in the .tcx file. Any .tcx file contains at least one lap (so 
       strictly speaking any Run can be seen as a Workout). The laps show up in the file according to the settings on the watch
       and/or if the file corresponds to a predefined workout. */
public:
    Workout(std::ifstream& infile);
    Workout(const std::string& dir);

    Run getLap(int lapNumber);

    std::vector<int> getStartTimes();

private:
    std::vector<TrackVecIt> m_lapIts;
    std::vector<int> m_startTimes;
};

#endif