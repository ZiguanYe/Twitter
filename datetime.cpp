#include <ctime>
#include <iomanip>
#include <sstream>
#include "datetime.h"

using namespace std;

DateTime::DateTime() {
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // Use timeinfo pointer to access fields of the tm struct
    year = timeinfo->tm_year + 1900;
    month = timeinfo->tm_mon + 1;
    day = timeinfo->tm_mday;
    
    hour = timeinfo->tm_hour;
    min = timeinfo->tm_min;
    sec = timeinfo->tm_sec;
}

DateTime::DateTime(int hh, int mm, int ss, int year, int month, int day) : hour(hh), min(mm), sec(ss), year(year), month(month), day(day)
{
}

bool DateTime::operator<(const DateTime &other) const
{
    if (year < other.year)
    {
        return true;
    }
    else if (year == other.year && month < other.month)
    {
        return true;
    }
    else if (year == other.year && month == other.month && day < other.day)
    {
        return true;
    }
    else if (year == other.year && month == other.month && day == other.day && hour < other.hour)
    {
        return true;
    }
    else if (year == other.year && month == other.month && day == other.day && hour == other.hour && min < other.min)
    {
        return true;
    }
    else if (year == other.year && month == other.month && day == other.day && hour == other.hour && min == other.min && sec < other.sec)
    {
        return true;
    }
    else
    {
        return false;
    }
}

ostream &operator<<(ostream &os, const DateTime &other)
{
    //setfill to fill the digits
    os << setfill('0') << setw(4) << other.year << "-" << setw(2)
       << other.month << "-" << setw(2)
       << other.day << " " << setw(2) << other.hour << ":" << setw(2)
       << other.min << ":" << setw(2) << other.sec;
    
    return os;
}

istream &operator>>(istream &is, DateTime &dt)
{
    string date;
    string time;
    is >> date;
    is >> time;
    
    if (is.fail())
    {
        dt = DateTime();
        return is;
    }
    
    stringstream ss(date);
    stringstream ss2(time);
    string year, month, day, hour, min, sec;
    
    try
    {
        //input date
        getline(ss, year, '-');
        dt.year = stoi(year);
        getline(ss, month, '-');
        dt.month = stoi(month);
        getline(ss, day, '-');
        dt.day = stoi(day);
        //input time
        getline(ss2, hour, ':');
        dt.hour = stoi(hour);
        getline(ss2, min, ':');
        dt.min = stoi(min);
        getline(ss2, sec, ':');
        dt.sec = stoi(sec);
    }
    catch (invalid_argument &)
    {
        dt = DateTime();
        is.setstate(std::ios_base::failbit);
    }
    return is;
}
