#include "Timetable.h"

using namespace std;


// Add a time slot to the timetable
void Timetable::addSlot(const TimeSlot& slot)
{
    slots.push_back(slot);
}


// Return all time slots
const vector<TimeSlot>& Timetable::getSlots() const
{
    return slots;
}


// Check whether this timetable clashes with another timetable
bool Timetable::clashWith(const Timetable& other) const
{
    for (const TimeSlot& mySlot : slots)
    {
        for (const TimeSlot& otherSlot : other.slots)
        {
            if (mySlot.overlaps(otherSlot))
            {
                return true;
            }
        }
    }

    return false;
}


// Add a time slot using += operator
Timetable& Timetable::operator+=(const TimeSlot& slot)
{
    addSlot(slot);

    return *this;
}