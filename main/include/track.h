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
    Track() : m_time {0}, m_distance {0.0}, m_altitude {0.0}, m_speed {0.0}, m_heartRate {0.0} { }

    Track(int time, float distance, float altitude, float speed, float heartRate = 0.0)
    : m_time {time}, m_distance {distance}, m_altitude {altitude}, m_speed {speed}, m_heartRate {heartRate}
    { }

    friend class Run;
    friend class Workout;
};


#endif