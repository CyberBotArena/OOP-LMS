#include "AttendanceSession.h"

#include <stdexcept>

using namespace std;


// Constructor
AttendanceSession::AttendanceSession(
    const string& session_ID,
    const string& offering_ID,
    const string& lecturer_ID,
    const TimeSlot& slot,
    int duration_minutes
)
    : session_ID(session_ID),
      offering_ID(offering_ID),
      open_by_lecturer_ID(lecturer_ID),
      opened_at(chrono::system_clock::now()),
      expires_at(opened_at + chrono::minutes(duration_minutes)),
      open(true),
      slot(slot)
{
    if (session_ID.empty())
    {
        throw invalid_argument(
            "Session ID cannot be empty."
        );
    }

    if (offering_ID.empty())
    {
        throw invalid_argument(
            "Offering ID cannot be empty."
        );
    }

    if (lecturer_ID.empty())
    {
        throw invalid_argument(
            "Lecturer ID cannot be empty."
        );
    }

    if (duration_minutes <= 0)
    {
        throw invalid_argument(
            "Session duration must be greater than zero."
        );
    }
}


// Get session ID
const string& AttendanceSession::getId() const
{
    return session_ID;
}


// Get course offering ID
const string& AttendanceSession::getOfferingId() const
{
    return offering_ID;
}


// Check whether the session has expired
bool AttendanceSession::hasExpired() const
{
    return chrono::system_clock::now() > expires_at;
}


// Check whether the session is currently open
bool AttendanceSession::isOpen() const
{
    return open && !hasExpired();
}


// Manually close the session
void AttendanceSession::close()
{
    open = false;
}