#include "timestamp.h"

#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
#include <vector>


TimeStamp::TimeStamp() {}

TimeStamp::TimeStamp(const std::string& time_stamp)
:   m_date { time_stamp.substr(0,10) },
    m_hour { std::stoi(time_stamp.substr(11,2)) },
    m_min  { std::stoi(time_stamp.substr(14,2)) },
    m_sec  { std::stoi(time_stamp.substr(17,2)) }
{
}

TimeStamp::TimeStamp(const std::string& date, const std::string& time)
:   m_date { date },
    m_hour { std::stoi(time.substr(0,2)) },
    m_min  { std::stoi(time.substr(3,2)) },
    m_sec  { std::stoi(time.substr(6,2)) }
{   
}

TimeStamp::TimeStamp(const std::string& date, int zero)
:   m_date { date },
    m_hour { 0 },
    m_min  { 0 },
    m_sec  { 0 }
{
}

int TimeStamp::getYear() const { return std::stoi(m_date.substr(0,4)); }
int TimeStamp::getMonth() const { return std::stoi(m_date.substr(5,2)); }
int TimeStamp::getDay()  const { return std::stoi(m_date.substr(8,2)); }
std::string TimeStamp::getDate(){ return m_date; }

int TimeStamp::secondsPast(int initial_time) const
{
    return m_hour*3600 + m_min*60 + m_sec - initial_time;
}

int TimeStamp::secondsPast(const TimeStamp& time_stamp) const
{
    return secondsPast(time_stamp.secondsPast(0));
}

//checks if the *this TimeStamp is earlier than stamp.
bool TimeStamp::isBefore(const TimeStamp& stamp) const
{
    if (m_date == stamp.m_date){ return secondsPast(0) < stamp.secondsPast(0); }

    if(getYear() == stamp.getYear())
    {
        if(getMonth() == stamp.getMonth())
        {
            return getDay() < stamp.getDay();
        }
        else
        {
            return getMonth() < stamp.getMonth();
        }
    }
    else 
    {
        return getYear() < stamp.getYear();
    }
}
