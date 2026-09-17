#ifndef ATTENDANCE_RECORD_H
#define ATTENDANCE_RECORD_H

#include <chrono>
#include <string>

enum AttendanceStatus {
    Present,
    Absent,
    Late,
    Excused
};

class AttendanceRecord {
private:
    std::string record_ID;
    std::string student_ID;
    std::string session_ID;

    std::chrono::system_clock::time_point time_stamp;

    AttendanceStatus status;
    std::string capture_method;

    // Used only when this record is a correction
    std::string correction_of;
    std::string acting_lecturer_ID;
    std::string reason;

public:
    AttendanceRecord(
        const std::string& record_ID,
        const std::string& student_ID,
        const std::string& session_ID,
        AttendanceStatus status,
        const std::string& capture_method,
        const std::string& correction_of = "",
        const std::string& acting_lecturer_ID = "",
        const std::string& reason = ""
    );

    const std::string& getRecordID() const;
    const std::string& getStudentID() const;
    const std::string& getSessionID() const;

    AttendanceStatus getStatus() const;

    const std::string& getCaptureMethod() const;
    const std::string& getCorrectionOf() const;
    const std::string& getActingLecturerID() const;
    const std::string& getReason() const;

    std::chrono::system_clock::time_point getTimestamp() const;

    bool isCorrection() const;
};

#endif