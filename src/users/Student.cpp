#include "users/Student.h"

#include <algorithm>
#include <iostream>

using namespace std;


// Constructor
Student::Student(
    const string& user_Id,
    const string& name,
    const string& user_name,
    const string& password
)
    : User(user_Id, name, user_name, password)
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
void Student::getMenu() const
{
    cout << "\n================================\n";
        cout << "           STUDENT MENU\n";
        cout << "================================\n";

        cout << "Welcome, "
        << getName() << '\n';

        cout << "Student ID: "
        << getUserId() << "\n\n";
        cout << "1. View available offerings\n";
        cout << "2. Enrol in an offering\n";
        cout << "3. View my enrolments\n";
        cout << "4. View my timetable\n";
        cout << "5. Drop course\n";
        cout << "0. Logout\n";

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