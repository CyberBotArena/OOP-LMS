#ifndef ATTENDANCE_REGISTER_H
#define ATTENDANCE_REGISTER_H

#include <string>
#include <vector>

#include "AttendanceRecord.h"
#include "AttendanceSession.h"

class AttendanceRegister {
private:
    std::vector<AttendanceSession> sessions;
    std::vector<AttendanceRecord> records;

public:
    // Session management
    void addSession(const AttendanceSession& session);
    void closeSession(const std::string& sessionId);

    AttendanceSession* findSession(const std::string& sessionId);
    const AttendanceSession* findSession(const std::string& sessionId) const;

    // Record management
    void addRecord(const AttendanceRecord& record);

    AttendanceRecord* findRecord(const std::string& recordId);
    const AttendanceRecord* findRecord(const std::string& recordId) const;

    // Accessors
    const std::vector<AttendanceSession>& getSessions() const;
    const std::vector<AttendanceRecord>& getRecords() const;

    // Attendance operations
    void markPresent(
        const std::string& record_ID,
        const std::string& student_ID,
        const std::string& session_ID,
        const std::string& capture_method
    );

    void appendCorrection(
        const std::string& new_record_ID,
        const std::string& original_record_ID,
        AttendanceStatus new_status,
        const std::string& lecturer_ID,
        const std::string& reason
    );

    // Reporting
    double percentageFor(
        const std::string& student_ID,
        const std::string& offering_ID
    ) const;
};

#endif