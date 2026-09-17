#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"
#include "../scheduling/Timetable.h"

#include <string>
#include <vector>

class Student : public User {
private:
    std::vector<std::string> enrolledCourseCodes;

    std::vector<std::string> completedCourses;

    Timetable personalTimetable;

public:
    Student(
        const std::string& name,
        const std::string& username,
        const std::string& password
    );

    ~Student() override;

    std::string getRole() const override;

    void showDashboard() const override;


    // Current enrolments

    const std::vector<std::string>&
    getEnrolledCourses() const;

    bool isEnrolledIn(
        const std::string& courseCode
    ) const;

    void addEnrolledCourse(
        const std::string& courseCode
    );

    void removeEnrolledCourse(
        const std::string& courseCode
    );


    // Completed courses

    void addCompletedCourse(
        const std::string& courseCode
    );

    bool hasCompletedCourse(
        const std::string& courseCode
    ) const;

    const std::vector<std::string>&
    getCompletedCourses() const;


    // Student timetable

    Timetable& getPersonalTimetable();

    const Timetable&
    getPersonalTimetable() const;
};

#endif