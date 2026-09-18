#include "Lecturer.h"

#include <algorithm>
#include <iostream>

using namespace std;

int Lecturer::next_lecturer_num = 1001;


// Constructor
Lecturer::Lecturer(
    //const string& user_Id,
    const string& name,
    const string& username,
    const string& password
)
    : User(name, username, password)
{
}


// Destructor
Lecturer::~Lecturer()
{
}


// generate student id
string Lecturer::generateLecturerId()
{
    return "LEC" + to_string(next_lecturer_num++);
}

// Return role
string Lecturer::getRole() const
{
    return "Lecturer";
}


// Lecturer menu
void Lecturer::getMenu() const
{
    cout << "--- Lecturer Dashboard: "
         << getName()
         << " ---\n"

         << "1. View my courses\n"
         << "2. View enrolment list for a course\n"
         << "3. Open/close an attendance session\n"
         << "4. Mark/correct attendance\n"
         << "5. View attendance reports\n"
         << "6. Logout\n";
}


// Get assigned courses
const vector<string>&
Lecturer::getAssignedCourses() const
{
    return assignedCourseCodes;
}


// Check whether lecturer teaches course
bool Lecturer::teaches(
    const string& courseCode) const
{
    return find(
        assignedCourseCodes.begin(),
        assignedCourseCodes.end(),
        courseCode
    ) != assignedCourseCodes.end();
}


// Assign course
void Lecturer::assignCourse(
    const string& courseCode)
{
    if (!teaches(courseCode))
    {
        assignedCourseCodes.push_back(
            courseCode
        );
    }
}


// Remove assigned course
void Lecturer::unassignCourse(
    const string& courseCode)
{
    assignedCourseCodes.erase(
        remove(
            assignedCourseCodes.begin(),
            assignedCourseCodes.end(),
            courseCode
        ),
        assignedCourseCodes.end()
    );
}