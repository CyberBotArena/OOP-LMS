#ifndef UNIVERSITY_EXCEPTION_H
#define UNIVERSITY_EXCEPTION_H

#include <stdexcept>
#include <string>

class UniversityException : public std::runtime_error
{
public:
    explicit UniversityException(const std::string& message)
        : std::runtime_error(message)
    {
    }
};

#endif