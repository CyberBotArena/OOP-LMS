#ifndef TIME_SLOT_H
#define TIME_SLOT_H

#include <string>

enum class Day
{
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday
};

class TimeSlot {
private:
    Day day;
    int startMinutes;
    int endMinutes;
    std::string location;

public:
    TimeSlot(
        Day day,
        int startMinutes,
        int endMinutes,
        const std::string& location
    );

    Day getDay() const;

    int getStartMinutes() const;

    int getEndMinutes() const;

    const std::string& getLocation() const;

    bool overlaps(const TimeSlot& other) const;

    bool operator==(const TimeSlot& other) const;
};

#endif