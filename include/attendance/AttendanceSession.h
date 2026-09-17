#ifndef ATTENDANCE_SESSION_H
#define ATTENDANCE_SESSION_H

#include <chrono>
#include <string>

#include "../scheduling/Timeslot.h"

class AttendanceSession {
private:
    std::string session_ID;
    std::string offering_ID;
    std::string open_by_lecturer_ID;

    std::chrono::system_clock::time_point opened_at;
    std::chrono::system_clock::time_point expires_at;

    bool open;
    TimeSlot slot;

public:
    AttendanceSession(
        const std::string& session_ID,
        const std::string& offering_ID,
        const std::string& lecturer_ID,
        const TimeSlot& slot,
        int duration_minutes
    );

    const std::string& getId() const;
    const std::string& getOfferingId() const;

    bool hasExpired() const;
    bool isOpen() const;

    void close();
};

#endif