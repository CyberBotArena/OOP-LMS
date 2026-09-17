#include "AttendanceRegister.h"

#include <stdexcept>

using namespace std;


// Add a new attendance session
void AttendanceRegister::addSession(
    const AttendanceSession& session)
{
    if (findSession(session.getId()) != nullptr)
    {
        throw invalid_argument(
            "A session with this ID already exists."
        );
    }

    sessions.push_back(session);
}


// Add a new attendance record
void AttendanceRegister::addRecord(
    const AttendanceRecord& record)
{
    if (findRecord(record.getRecordID()) != nullptr)
    {
        throw invalid_argument(
            "A record with this ID already exists."
        );
    }

    AttendanceSession* session =
        findSession(record.getSessionID());

    // Important: check nullptr first
    if (session == nullptr)
    {
        throw invalid_argument(
            "Attendance session does not exist."
        );
    }

    if (!session->isOpen())
    {
        throw runtime_error(
            "Cannot add attendance to a closed or expired session."
        );
    }

    records.push_back(record);
}


// Find a session
AttendanceSession* AttendanceRegister::findSession(
    const string& sessionId)
{
    for (AttendanceSession& session : sessions)
    {
        if (session.getId() == sessionId)
        {
            return &session;
        }
    }

    return nullptr;
}


// Read-only findSession
const AttendanceSession* AttendanceRegister::findSession(
    const string& sessionId) const
{
    for (const AttendanceSession& session : sessions)
    {
        if (session.getId() == sessionId)
        {
            return &session;
        }
    }

    return nullptr;
}


// Find an attendance record
AttendanceRecord* AttendanceRegister::findRecord(
    const string& recordId)
{
    for (AttendanceRecord& record : records)
    {
        if (record.getRecordID() == recordId)
        {
            return &record;
        }
    }

    return nullptr;
}


// Read-only findRecord
const AttendanceRecord* AttendanceRegister::findRecord(
    const string& recordId) const
{
    for (const AttendanceRecord& record : records)
    {
        if (record.getRecordID() == recordId)
        {
            return &record;
        }
    }

    return nullptr;
}


// Close an attendance session
void AttendanceRegister::closeSession(
    const string& sessionId)
{
    AttendanceSession* session =
        findSession(sessionId);

    if (session == nullptr)
    {
        throw invalid_argument(
            "The attendance session does not exist."
        );
    }

    session->close();
}


// Return all sessions
const vector<AttendanceSession>&
AttendanceRegister::getSessions() const
{
    return sessions;
}


// Return all attendance records
const vector<AttendanceRecord>&
AttendanceRegister::getRecords() const
{
    return records;
}


// Mark student as present
void AttendanceRegister::markPresent(
    const string& record_ID,
    const string& student_ID,
    const string& session_ID,
    const string& capture_method)
{
    AttendanceRecord newRecord(
        record_ID,
        student_ID,
        session_ID,
        AttendanceStatus::Present,
        capture_method
    );

    addRecord(newRecord);
}


// Append a correction record
void AttendanceRegister::appendCorrection(
    const string& new_record_ID,
    const string& original_record_ID,
    AttendanceStatus new_status,
    const string& lecturer_ID,
    const string& reason)
{
    AttendanceRecord* originalRecord =
        findRecord(original_record_ID);

    if (originalRecord == nullptr)
    {
        throw invalid_argument(
            "Original attendance record does not exist."
        );
    }

    AttendanceRecord correctionRecord(
        new_record_ID,
        originalRecord->getStudentID(),
        originalRecord->getSessionID(),
        new_status,
        "Lecturer Correction",
        original_record_ID,
        lecturer_ID,
        reason
    );

    // Your old version was missing this
    addRecord(correctionRecord);
}


// Calculate attendance percentage
double AttendanceRegister::percentageFor(
    const string& student_ID,
    const string& offering_ID) const
{
    int total_sessions = 0;
    int attended_sessions = 0;

    for (const AttendanceSession& session : sessions)
    {
        if (session.getOfferingId() == offering_ID)
        {
            total_sessions++;

            // Search backwards so the latest
            // record/correction is used
            for (int i =
                     static_cast<int>(records.size()) - 1;
                 i >= 0;
                 i--)
            {
                const AttendanceRecord& record =
                    records[i];

                if (record.getStudentID() == student_ID &&
                    record.getSessionID() == session.getId())
                {
                    if (record.getStatus() ==
                            AttendanceStatus::Present ||
                        record.getStatus() ==
                            AttendanceStatus::Late)
                    {
                        attended_sessions++;
                    }

                    break;
                }
            }
        }
    }

    if (total_sessions == 0)
    {
        return 0.0;
    }

    return (
        static_cast<double>(attended_sessions) /
        total_sessions
    ) * 100.0;
}