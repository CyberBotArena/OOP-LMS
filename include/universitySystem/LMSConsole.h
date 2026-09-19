#ifndef LMS_CONSOLE_H
#define LMS_CONSOLE_H

#include <string>

#include "universitySystem/UniversitySystem.h"
#include "enrolment/EnrolmentService.h"

class Student;
class Lecturer;
class Administrator;
class CourseOffering;


class LMSConsole {
private:
    UniversitySystem& system;
    EnrolmentService enrolmentService;

    void createTestData();

    void runStudentMenu(Student& student);
    void runLecturerMenu(Lecturer& lecturer);
    void runAdministratorMenu(Administrator& admin);

    void viewAvailableOfferings() const;
    void enrolStudent(Student& student);
    void viewMyEnrolments(const Student& student) const;
    void viewMyTimetable(const Student& student) const;

    void viewLecturerCourses(const Lecturer& lecturer) const;
    void viewEnrolmentList(const Lecturer& lecturer)const;
    //void viewOfferingEnrolments(const Lecturer& lecturer) const;
    void viewAttendanceReport(const Lecturer& lecturer) const;

    void manageAttendanceRecords(Lecturer& lecturer);
    void manageAttendanceSession(Lecturer& lecturer);

    void viewAllUsers() const;
    void viewAllCourses() const;
    void createCourse();
    void createOffering();
    void addTimetableSlot();

    static std::string dayToString(Day day);
    static std::string minutesToTime(int minutes);
    static int timeToMinutes(const std::string& time);
    static bool readInteger(int& value);

public:
    explicit LMSConsole(UniversitySystem& universitySystem);
    void run();
};

#endif
