#ifndef RUNCLASSES_H
#define RUNCLASSES_H

#include "readers.h"

#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <algorithm>


class BasicRun
{
private:
    std::string m_id;
    float m_totalTime; //{0.0}; //in minutes
    float m_totalDist; //{0.0}; //in km

public:
    BasicRun();
    BasicRun(std::ifstream& infile);    // Initialize with filestream object
    BasicRun(const std::string& str);   // or wiht directory string

    std::string getId()  const;
    float getTotalTime() const;
    float getTotalDist() const;

    void print() const;
};


class StandardRun // : public BasicRun 
{
protected:
    std::string m_id;
    std::vector<int> m_times;
    std::vector<float> m_distances;
    std::vector<float> m_altitudes;
    std::vector<float> m_speeds;

    // We want collect user-defined laps (based on distance)
    struct Lap
    {
    float distance;
    float time;
    };
    
    // This function collects the lap data
    std::vector<Lap> getLaps(int laplength = 1000);

public:
    StandardRun();                          
    StandardRun(std::ifstream& infile);     // Initialize with filestream object
    StandardRun(const std::string& str);    // or with directory string

    std::string getId() const;
    std::vector<int> getTimes();
    std::vector<float> getDistances();
    std::vector<float> getSpeeds();
    std::vector<float> getAltitudes();
    std::vector<float> getLapTimes(int k = 1000);   
    std::vector<float> getLapDistances(int k = 1000);

    std::string getDate() const;
    float getTotalTime() const;
    float getTotalDistance() const;
    float getAvgSpeed() const;

};


class Workout : public StandardRun
{
private:
    std::vector<float> m_lapTimes;
    std::vector<float> m_lapDistances;

public:
    Workout(std::ifstream& infile);

    std::vector<float> getLapTimes();
    std::vector<float> getLapDistances();
};


#endif
