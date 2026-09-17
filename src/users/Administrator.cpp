#ifndef COURSE_OFFERING_H
#define COURSE_OFFERING_H

#include <iostream>
#include <string>
#include <vector>

#include "../scheduling/Timetable.h"
#include "../attendance/AttendanceRegister.h"

// Forward declarations
class Student;
class Lecturer;
class Course;

class CourseOffering {
private:
    std::string offeringId;
    std::string semester;
    int capacity;

    // Aggregation - CourseOffering does not own these objects
    std::vector<Student*> enrolled;
    Lecturer* assignedLecturer;
    Course* course;

    // Composition
    Timetable timetable;
    AttendanceRegister attendance;

public:
    CourseOffering(
        const std::string& id,
        const std::string& semester,
        int capacity
    );

    // Student enrolment
    void addStudent(Student* student);

    void removeStudent(
        const std::string& studentId
    );

    bool isStudentEnrolled(
        const std::string& studentId
    ) const;

    bool isFull() const;


    // Basic getters
    const std::string& getOfferingId() const;

    const std::string& getSemester() const;

    int getCapacity() const;

    std::size_t getEnrolledCount() const;

    const std::vector<Student*>&
    getEnrolledStudents() const;


    // Timetable
    const Timetable& getTimetable() const;

    Timetable& getTimetable();

    void setTimetable(
        const Timetable& timetable
    );


    // Attendance
    AttendanceRegister& getAttendanceRegister();

    const AttendanceRegister&
    getAttendanceRegister() const;


    // Lecturer
    void setLecturer(Lecturer* lecturer);

    Lecturer* getLecturer() const;


    // Course
    void setCourse(Course* course);

    Course* getCourse() const;


    // Output operator
    friend std::ostream& operator<<(
        std::ostream& out,
        const CourseOffering& offering
    );
};

#endif