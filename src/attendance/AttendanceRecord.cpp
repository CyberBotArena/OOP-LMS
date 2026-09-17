#include "AttendanceRecord.h"

#include <stdexcept>

using namespace std;


// Constructor
AttendanceRecord::AttendanceRecord(
    const string& record_ID,
    const string& student_ID,
    const string& session_ID,
    AttendanceStatus status,
    const string& capture_method,
    const string& correction_of,
    const string& acting_lecturer_ID,
    const string& reason
)
    : record_ID(record_ID),
      student_ID(student_ID),
      session_ID(session_ID),
      time_stamp(chrono::system_clock::now()),
      status(status),
      capture_method(capture_method),
      correction_of(correction_of),
      acting_lecturer_ID(acting_lecturer_ID),
      reason(reason)
{
    if (record_ID.empty()) {
        throw invalid_argument("Record ID cannot be empty.");
    }

    if (student_ID.empty()) {
        throw invalid_argument("Student ID cannot be empty.");
    }

    if (session_ID.empty()) {
        throw invalid_argument("Session ID cannot be empty.");
    }

    if (capture_method.empty()) {
        throw invalid_argument("Capture method cannot be empty.");
    }

    // If this is a correction record,
    // lecturer ID and reason are required.
    if (!correction_of.empty() &&
        (acting_lecturer_ID.empty() || reason.empty())) {

        throw invalid_argument(
            "Correction requires lecturer ID and reason."
        );
    }
}


// Get record ID
const string& AttendanceRecord::getRecordID() const
{
    return record_ID;
}


// Get student ID
const string& AttendanceRecord::getStudentID() const
{
    return student_ID;
}


// Get session ID
const string& AttendanceRecord::getSessionID() const
{
    return session_ID;
}


// Get attendance status
AttendanceStatus AttendanceRecord::getStatus() const
{
    return status;
}


// Get capture method
const string& AttendanceRecord::getCaptureMethod() const
{
    return capture_method;
}


// Get the original record ID
const string& AttendanceRecord::getCorrectionOf() const
{
    return correction_of;
}


// Get lecturer who made the correction
const string& AttendanceRecord::getActingLecturerID() const
{
    return acting_lecturer_ID;
}


// Get reason for correction
const string& AttendanceRecord::getReason() const
{
    return reason;
}


// Get timestamp
chrono::system_clock::time_point
AttendanceRecord::getTimestamp() const
{
    return time_stamp;
}


// Check whether this record is a correction
bool AttendanceRecord::isCorrection() const
{
    return !correction_of.empty();
}