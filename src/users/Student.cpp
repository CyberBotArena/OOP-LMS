#include "Student.h"

#include <algorithm>
#include <iostream>

using namespace std;


// Constructor
Student::Student(
    const string& name,
    const string& username,
    const string& password
)
    : User(name, username, password)
{
}


// Destructor
Student::~Student()
{
}


// Return role
string Student::getRole() const
{
    return "Student";
}


// Student menu
void Student::showDashboard() const
{
    cout << "--- Student Dashboard: "
         << getName()
         << " ---\n"

         << "1. View available courses\n"
         << "2. Enrol in a course\n"
         << "3. Drop a course\n"
         << "4. View my timetable\n"
         << "5. View my attendance record\n"
         << "6. Logout\n";
}


// Get currently enrolled courses
const vector<string>&
Student::getEnrolledCourses() const
{
    return enrolledCourseCodes;
}


// Check whether student is enrolled
bool Student::isEnrolledIn(
    const string& courseCode) const
{
    return find(
        enrolledCourseCodes.begin(),
        enrolledCourseCodes.end(),
        courseCode
    ) != enrolledCourseCodes.end();
}


// Add current enrolment
void Student::addEnrolledCourse(
    const string& courseCode)
{
    if (!isEnrolledIn(courseCode))
    {
        enrolledCourseCodes.push_back(
            courseCode
        );
    }
}


// Remove current enrolment
void Student::removeEnrolledCourse(
    const string& courseCode)
{
    enrolledCourseCodes.erase(
        remove(
            enrolledCourseCodes.begin(),
            enrolledCourseCodes.end(),
            courseCode
        ),
        enrolledCourseCodes.end()
    );
}


// Add a completed course
void Student::addCompletedCourse(
    const string& courseCode)
{
    if (!hasCompletedCourse(courseCode))
    {
        completedCourses.push_back(
            courseCode
        );
    }
}


// Check whether prerequisite course was completed
bool Student::hasCompletedCourse(
    const string& courseCode) const
{
    return find(
        completedCourses.begin(),
        completedCourses.end(),
        courseCode
    ) != completedCourses.end();
}


// Get completed courses
const vector<string>&
Student::getCompletedCourses() const
{
    return completedCourses;
}


// Get editable timetable
Timetable& Student::getPersonalTimetable()
{
    return personalTimetable;
}


// Get read-only timetable
const Timetable&
Student::getPersonalTimetable() const
{
    return personalTimetable;
}