#ifndef ENROLMENT_EXCEPTIONS_H
#define ENROLMENT_EXCEPTIONS_H

#include <stdexcept>
#include <string>

class CourseFullException : public std::runtime_error {
public:
    explicit CourseFullException(const std::string& msg) : std::runtime_error(msg) {}
};

class ScheduleClashException : public std::runtime_error {
public:
    explicit ScheduleClashException(const std::string& msg) : std::runtime_error(msg) {}
};

class AlreadyEnrolledException : public std::runtime_error {
public:
    explicit AlreadyEnrolledException(const std::string& msg) : std::runtime_error(msg) {}
};

#endif