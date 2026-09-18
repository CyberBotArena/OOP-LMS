#include "CourseOffering.h"

#include "users/Student.h"
#include "users/Lecturer.h"
#include "Course.h"

#include <stdexcept>

using namespace std;


// Constructor
CourseOffering::CourseOffering(
    const string& id,
    const string& semester,
    int capacity
)
    : offeringId(id),
      semester(semester),
      capacity(capacity),
      assignedLecturer(nullptr),
      course(nullptr)
{
    if (offeringId.empty())
    {
        throw invalid_argument(
            "Offering ID cannot be empty."
        );
    }

    if (semester.empty())
    {
        throw invalid_argument(
            "Semester cannot be empty."
        );
    }

    if (capacity <= 0)
    {
        throw invalid_argument(
            "Capacity must be greater than zero."
        );
    }
}


// Add student
void CourseOffering::addStudent(Student* student)
{
    if (student == nullptr)
    {
        throw invalid_argument(
            "Student cannot be null."
        );
    }

    if (isStudentEnrolled(
            to_string(student->getUserId())))
    {
        throw runtime_error(
            "Student is already enrolled."
        );
    }

    if (isFull())
    {
        throw runtime_error(
            "Course offering is full."
        );
    }

    enrolled.push_back(student);
}


// Remove student
void CourseOffering::removeStudent(
    const string& studentId)
{
    for (auto it = enrolled.begin();
         it != enrolled.end();
         ++it)
    {
        if (to_string((*it)->getUserId())
            == studentId)
        {
            enrolled.erase(it);
            return;
        }
    }

    throw runtime_error(
        "Student is not enrolled in this offering."
    );
}


// Check whether student is enrolled
bool CourseOffering::isStudentEnrolled(
    const string& studentId) const
{
    for (const Student* student : enrolled)
    {
        if (student != nullptr &&
            to_string(student->getUserId())
                == studentId)
        {
            return true;
        }
    }

    return false;
}


// Check capacity
bool CourseOffering::isFull() const
{
    return enrolled.size()
           >= static_cast<size_t>(capacity);
}


// Get offering ID
const string&
CourseOffering::getOfferingId() const
{
    return offeringId;
}


// Get semester
const string&
CourseOffering::getSemester() const
{
    return semester;
}


// Get maximum capacity
int CourseOffering::getCapacity() const
{
    return capacity;
}


// Get number of enrolled students
size_t CourseOffering::getEnrolledCount() const
{
    return enrolled.size();
}


// Get enrolled students
const vector<Student*>&
CourseOffering::getEnrolledStudents() const
{
    return enrolled;
}


// Get read-only timetable
const Timetable&
CourseOffering::getTimetable() const
{
    return timetable;
}


// Get editable timetable
Timetable&
CourseOffering::getTimetable()
{
    return timetable;
}


// Replace timetable
void CourseOffering::setTimetable(
    const Timetable& newTimetable)
{
    timetable = newTimetable;
}


// Get attendance register
AttendanceRegister&
CourseOffering::getAttendanceRegister()
{
    return attendance;
}


// Get read-only attendance register
const AttendanceRegister&
CourseOffering::getAttendanceRegister() const
{
    return attendance;
}


// Assign lecturer
void CourseOffering::setLecturer(
    Lecturer* lecturer)
{
    assignedLecturer = lecturer;
}


// Get lecturer
Lecturer*
CourseOffering::getLecturer() const
{
    return assignedLecturer;
}


// Set course
void CourseOffering::setCourse(
    Course* newCourse)
{
    course = newCourse;
}


// Get course
Course*
CourseOffering::getCourse() const
{
    return course;
}


// Output operator
ostream& operator<<(
    ostream& out,
    const CourseOffering& offering)
{
    out << "Offering ID: "
        << offering.offeringId
        << " | Semester: "
        << offering.semester
        << " | Capacity: "
        << offering.capacity
        << " | Enrolled: "
        << offering.enrolled.size();

    return out;
}