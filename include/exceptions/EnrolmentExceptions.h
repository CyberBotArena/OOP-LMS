#ifndef ENROLMENT_EXCEPTIONS_H
#define ENROLMENT_EXCEPTIONS_H

#include "exceptions/UniversityException.h"


// Base class for all enrolment-related errors
class EnrolmentException : public UniversityException
{
public:
    explicit EnrolmentException(const std::string& message)
        : UniversityException(message)
    {
    }
};


// Course offering has no available space
class CourseFullException : public EnrolmentException
{
public:
    explicit CourseFullException(const std::string& message)
        : EnrolmentException(message)
    {
    }
};


// Student is already enrolled
class AlreadyEnrolledException : public EnrolmentException
{
public:
    explicit AlreadyEnrolledException(const std::string& message)
        : EnrolmentException(message)
    {
    }
};


// Student has not completed a required course
class PrerequisiteNotMetException : public EnrolmentException
{
public:
    explicit PrerequisiteNotMetException(const std::string& message)
        : EnrolmentException(message)
    {
    }
};


// New offering clashes with student's timetable
class TimetableClashException : public EnrolmentException
{
public:
    explicit TimetableClashException(const std::string& message)
        : EnrolmentException(message)
    {
    }
};

#endif