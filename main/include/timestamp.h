#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
#include <vector>

// A TimeStamp holds the time and date of a run. Made this to easily choose and sort files by time and date.
class TimeStamp
{
private:
    std::string m_date;
    int m_hour;
    int m_min;
    int m_sec;

public:
    TimeStamp();

    TimeStamp(const std::string& time_stamp);

    TimeStamp(const std::string& date, const std::string& time);

    TimeStamp(const std::string& date, int zero);

    int getYear() const;
    int getMonth() const;
    int getDay()  const;
    std::string getDate();

    int secondsPast(int initial_time = 0) const;

    int secondsPast(const TimeStamp& time_stamp) const;

    // checks if the *this TimeStamp is earlier than stamp.
    bool isBefore(const TimeStamp& stamp) const;

};




#endif