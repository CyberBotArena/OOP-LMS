#include "Timeslot.h"

#include <stdexcept>

using namespace std;


// Constructor
TimeSlot::TimeSlot(
    Day day,
    int startMinutes,
    int endMinutes,
    const string& location
)
    : day(day),
      startMinutes(startMinutes),
      endMinutes(endMinutes),
      location(location)
{
    if (startMinutes < 0 || endMinutes > 24 * 60)
    {
        throw invalid_argument(
            "Time must be between 00:00 and 24:00."
        );
    }

    if (startMinutes >= endMinutes)
    {
        throw invalid_argument(
            "Start time must be earlier than end time."
        );
    }

    if (location.empty())
    {
        throw invalid_argument(
            "Location cannot be empty."
        );
    }
}


// Get day
Day TimeSlot::getDay() const
{
    return day;
}


// Get start time in minutes
int TimeSlot::getStartMinutes() const
{
    return startMinutes;
}


// Get end time in minutes
int TimeSlot::getEndMinutes() const
{
    return endMinutes;
}


// Get location
const string& TimeSlot::getLocation() const
{
    return location;
}


// Check whether two time slots overlap
bool TimeSlot::overlaps(const TimeSlot& other) const
{
    if (day != other.day)
    {
        return false;
    }

    return startMinutes < other.endMinutes &&
           other.startMinutes < endMinutes;
}


// Check whether two time slots are exactly equal
bool TimeSlot::operator==(const TimeSlot& other) const
{
    return day == other.day &&
           startMinutes == other.startMinutes &&
           endMinutes == other.endMinutes &&
           location == other.location;
}