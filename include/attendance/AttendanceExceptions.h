#ifndef ATTENDANCE_EXCEPTIONS_H
#define ATTENDANCE_EXCEPTIONS_H

#include <stdexcept>
#include <string>

// Base Exception for Attendance System
class AttendanceException : public std::runtime_error {
public:
    explicit AttendanceException(const std::string& message)
        : std::runtime_error(message) {}
};

// Invalid Argument Specific Exception
class InvalidAttendanceDataException : public AttendanceException {
public:
    explicit InvalidAttendanceDataException(const std::string& message)
        : AttendanceException(message) {}
};

#endif
