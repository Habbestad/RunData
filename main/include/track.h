#ifndef TRACK_H
#define TRACK_H

#include "runclasses.h"

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
    Track() = default;

    Track(int time, float distance, float altitude, float speed, float heartRate = 0.0)
    : m_time {time}, m_distance {distance}, m_altitude {altitude}, m_speed {speed}, m_heartRate {heartRate}
    { }

    void setFromTcxSection(std::ifstream& tcxSection, int initialTimeOfRunInSeconds)
    {
        std::string time_str = tcxParsing::getData(tcxSection, "Time", "Trackpoint", true);
        m_time = TimeStamp(time_str).secondsPast(initialTimeOfRunInSeconds); 

        m_distance = std::stof(tcxParsing::getData(tcxSection, "DistanceMeters", "Trackpoint", true));
        m_speed = std::stof(tcxParsing::getData(tcxSection, "ns3:Speed", "Trackpoint", true));
        m_altitude = std::stof(tcxParsing::getData(tcxSection, "AltitudeMeters", "Trackpoint", false)); 
    }

    friend class Run;
    friend class Workout;
};


#endif