#ifndef COURSE_OFFERING_H
#define COURSE_OFFERING_H

#include <iostream>
#include <string>
#include <vector>

#include "../scheduling/Timetable.h"
#include "../attendance/AttendanceRegister.h"

class Student;
class Lecturer;
class Course;

class CourseOffering {
private:
    std::string offeringId;
    std::string semester;
    int capacity;

    std::vector<Student*> enrolled;
    Lecturer* assignedLecturer;

    Timetable timetable;
    AttendanceRegister attendance;

    Course* course;

public:
    CourseOffering(
        const std::string& id,
        const std::string& sem,
        int cap
    );

    void addStudent(Student* student);

    void removeStudent(
        const std::string& studentId
    );

    bool isFull() const;

    std::string getOfferingId() const;
    std::string getSemester() const;
    int getCapacity() const;
    size_t getEnrolledCount() const;

    const Timetable& getTimetable() const;

    void setTimetable(
        const Timetable& tt
    );

    AttendanceRegister& getAttendanceRegister();

    friend std::ostream& operator<<(
        std::ostream& out,
        const CourseOffering& offering
    );
};

#endif