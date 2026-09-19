#ifndef TIMETABLE_H
#define TIMETABLE_H

#include <vector>
#include "scheduling/Timeslot.h"

class Timetable {
private:
    std::vector<TimeSlot> slots;

public:
    void addSlot(const TimeSlot& slot);

    void removeSlot(const TimeSlot& slot);

    const std::vector<TimeSlot>& getSlots() const;

    bool clashWith(const Timetable& other) const;

    Timetable& operator+=(const TimeSlot& slot);
};

#endif