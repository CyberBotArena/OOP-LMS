#include "universitySystem/LMSConsole.h"

#include <iomanip>
#include <iostream>
#include <stdexcept>

#include "users/User.h"
#include "users/Student.h"
#include "users/Lecturer.h"
#include "users/Administrator.h"
#include "courses/Course.h"
#include "courses/LectureCourse.h"
#include "enrolment/CourseOffering.h"
#include "scheduling/Timeslot.h"

using namespace std;

LMSConsole::LMSConsole(UniversitySystem& universitySystem)
    : system(universitySystem)
{
}

bool LMSConsole::readInteger(int& value)
{
    if (cin >> value)
        return true;

    if (cin.eof())
        return false;

    cin.clear();
    cin.ignore(10000, '\n');
    return false;
}

string LMSConsole::dayToString(Day day)
{
    switch (day)
    {
        case Day::Monday: return "Monday";
        case Day::Tuesday: return "Tuesday";
        case Day::Wednesday: return "Wednesday";
        case Day::Thursday: return "Thursday";
        case Day::Friday: return "Friday";
        case Day::Saturday: return "Saturday";
        case Day::Sunday: return "Sunday";
    }
    return "Unknown";
}

string LMSConsole::minutesToTime(int minutes)
{
    int hours = minutes / 60;
    int mins = minutes % 60;
    string result;
    if (hours < 10) result += "0";
    result += to_string(hours) + ":";
    if (mins < 10) result += "0";
    result += to_string(mins);
    return result;
}

int LMSConsole::timeToMinutes(const string& time)
{
    if (time.length() != 5 || time[2] != ':')
        throw invalid_argument("Time must be in HH:MM format.");

    for (int i : {0, 1, 3, 4})
        if (time[i] < '0' || time[i] > '9')
            throw invalid_argument("Time must contain digits.");

    int hours = stoi(time.substr(0, 2));
    int minutes = stoi(time.substr(3, 2));

    if (hours > 23 || minutes > 59)
        throw invalid_argument("Invalid time value.");

    return hours * 60 + minutes;
}

void LMSConsole::createTestData()
{
    Student* student = new Student(system.generateUserId(), "Kamal Perera", "kamal", "1234");
    Lecturer* lecturer = new Lecturer(system.generateUserId(), "Nimal Silva", "nimal", "1234");
    Administrator* admin = new Administrator(system.generateUserId(), "Admin User", "admin", "admin123");

    system.addUser(student);
    system.addUser(lecturer);
    system.addUser(admin);

    LectureCourse* course = new LectureCourse("CS101", "Programming Fundamentals", 3);
    system.addCourse(course);

    CourseOffering* offering = new CourseOffering("OFF001", "Semester 1 - 2026", 30);
    offering->setCourse(course);
    offering->setLecturer(lecturer);
    lecturer->assignCourse(course->getCode());
    system.addOffering(offering);

    cout << "\nTest data created successfully.\n";
    cout << "Student ID: " << student->getUserId() << " | Password: 1234\n";
    cout << "Lecturer ID: " << lecturer->getUserId() << " | Password: 1234\n";
    cout << "Administrator ID: " << admin->getUserId() << " | Password: admin123\n";
}

void LMSConsole::viewAvailableOfferings() const
{
    cout << "\n================================\n       AVAILABLE OFFERINGS\n================================\n";
    vector<CourseOffering*> offerings = system.getOfferings();
    if (offerings.empty()) { cout << "No course offerings available.\n"; return; }

    for (CourseOffering* offering : offerings)
    {
        cout << "\nOffering ID: " << offering->getOfferingId() << '\n';
        cout << "Semester: " << offering->getSemester() << '\n';
        cout << "Capacity: " << offering->getEnrolledCount() << "/" << offering->getCapacity() << '\n';
        if (offering->getCourse())
            cout << "Course: " << offering->getCourse()->getCode() << " - " << offering->getCourse()->getTitle() << '\n';
        if (offering->getLecturer())
            cout << "Lecturer: " << offering->getLecturer()->getName() << '\n';
        cout << "------------------------------\n";
    }
}

void LMSConsole::enrolStudent(Student& student)
{
    string offeringId;
    cout << "\nEnter offering ID: ";
    cin >> offeringId;
    CourseOffering* offering = system.findOffering(offeringId);
    if (!offering) { cout << "Offering not found.\n"; return; }

    try
    {
        enrolmentService.enrol(student, *offering);
        cout << "Enrolment successful.\n";
    }
    catch (const exception& e)
    {
        cout << "Enrolment failed: " << e.what() << '\n';
    }
}

void LMSConsole::viewMyEnrolments(const Student& student) const
{
    cout << "\n================================\n          MY ENROLMENTS\n================================\n";
    bool found = false;
    for (CourseOffering* offering : system.getOfferings())
    {
        if (!student.isEnrolledIn(offering->getOfferingId())) continue;
        found = true;
        cout << "Offering ID: " << offering->getOfferingId();
        if (offering->getCourse())
            cout << " | " << offering->getCourse()->getCode() << " - " << offering->getCourse()->getTitle();
        cout << " | " << offering->getSemester() << '\n';
    }
    if (!found) cout << "You are not enrolled in any offerings.\n";
}

void LMSConsole::viewMyTimetable(const Student& student) const
{
    cout << "\n================================\n          MY TIMETABLE\n================================\n";
    const vector<TimeSlot>& slots = student.getPersonalTimetable().getSlots();
    if (slots.empty()) { cout << "No timetable slots available.\n"; return; }
    for (const TimeSlot& slot : slots)
        cout << dayToString(slot.getDay()) << " | " << minutesToTime(slot.getStartMinutes())
             << " - " << minutesToTime(slot.getEndMinutes()) << " | " << slot.getLocation() << '\n';
}

void LMSConsole::viewLecturerCourses(const Lecturer& lecturer) const
{
    cout << "\n================================\n        MY COURSE OFFERINGS\n================================\n";
    bool found = false;
    for (CourseOffering* offering : system.getOfferings())
    {
        if (offering->getLecturer() != &lecturer) continue;
        found = true;
        cout << offering->getOfferingId();
        if (offering->getCourse())
            cout << " | " << offering->getCourse()->getCode() << " - " << offering->getCourse()->getTitle();
        cout << " | " << offering->getSemester() << '\n';
    }
    if (!found) cout << "No course offerings are assigned to you.\n";
}

void LMSConsole::viewOfferingEnrolments(const Lecturer& lecturer) const
{
    string offeringId;
    cout << "Offering ID: ";
    cin >> offeringId;
    CourseOffering* offering = system.findOffering(offeringId);
    if (!offering || offering->getLecturer() != &lecturer)
    {
        cout << "Offering not found or not assigned to this lecturer.\n";
        return;
    }

    const vector<Student*>& students = offering->getEnrolledStudents();
    if (students.empty()) { cout << "No students are enrolled.\n"; return; }
    for (const Student* student : students)
        cout << student->getUserId() << " | " << student->getName() << '\n';
}

void LMSConsole::viewAttendanceReport(const Lecturer& lecturer) const
{
    string offeringId;
    cout << "Offering ID: ";
    cin >> offeringId;
    CourseOffering* offering = system.findOffering(offeringId);
    if (!offering || offering->getLecturer() != &lecturer)
    {
        cout << "Offering not found or not assigned to this lecturer.\n";
        return;
    }

    const vector<Student*>& students = offering->getEnrolledStudents();
    if (students.empty()) { cout << "No students are enrolled.\n"; return; }

    cout << fixed << setprecision(2);
    for (const Student* student : students)
    {
        double percentage = offering->getAttendanceRegister().percentageFor(
            student->getUserId(), offering->getOfferingId());
        cout << student->getUserId() << " | " << student->getName() << " | " << percentage << "%\n";
    }
}

void LMSConsole::viewAllUsers() const
{
    cout << "\nALL USERS\n";
    for (User* user : system.getAllUsers())
        cout << user->getUserId() << " | " << user->getName() << " | " << user->getRole() << '\n';
}

void LMSConsole::viewAllCourses() const
{
    cout << "\nALL COURSES\n";
    for (Course* course : system.getCourses())
        cout << course->getCode() << " | " << course->getTitle() << " | " << course->getCourseType() << '\n';
}

void LMSConsole::createCourse()
{
    string code, title;
    int credits;
    cout << "Course code: "; cin >> code;
    if (system.findCourse(code)) { cout << "A course with this code already exists.\n"; return; }
    cout << "Course title: "; cin >> ws; getline(cin, title);
    cout << "Credits: ";
    if (!readInteger(credits) || credits <= 0) { cout << "Invalid credit value.\n"; return; }

    try
    {
        LectureCourse* course = new LectureCourse(code, title, credits);
        system.addCourse(course);
        cout << "Course created successfully.\n";
    }
    catch (const exception& e) { cout << "Failed to create course: " << e.what() << '\n'; }
}

void LMSConsole::createOffering()
{
    string offeringId, courseCode, semester, lecturerId;
    int capacity;
    cout << "Offering ID: "; cin >> offeringId;
    if (system.findOffering(offeringId)) { cout << "This offering ID already exists.\n"; return; }
    cout << "Course code: "; cin >> courseCode;
    Course* course = system.findCourse(courseCode);
    if (!course) { cout << "Course not found.\n"; return; }
    cout << "Semester: "; cin >> ws; getline(cin, semester);
    cout << "Capacity: ";
    if (!readInteger(capacity) || capacity <= 0) { cout << "Invalid capacity.\n"; return; }
    cout << "Lecturer ID: "; cin >> lecturerId;
    Lecturer* lecturer = dynamic_cast<Lecturer*>(system.findUser(lecturerId));
    if (!lecturer) { cout << "Lecturer not found.\n"; return; }

    try
    {
        CourseOffering* offering = new CourseOffering(offeringId, semester, capacity);
        offering->setCourse(course);
        offering->setLecturer(lecturer);
        lecturer->assignCourse(course->getCode());
        system.addOffering(offering);
        cout << "Offering created successfully.\n";
    }
    catch (const exception& e) { cout << "Failed to create offering: " << e.what() << '\n'; }
}

void LMSConsole::addTimetableSlot()
{
    string offeringId;
    cout << "Offering ID: "; cin >> offeringId;
    CourseOffering* offering = system.findOffering(offeringId);
    if (!offering) { cout << "Offering not found.\n"; return; }

    cout << "1.Monday 2.Tuesday 3.Wednesday 4.Thursday 5.Friday 6.Saturday 7.Sunday\nDay: ";
    int dayNumber;
    if (!readInteger(dayNumber) || dayNumber < 1 || dayNumber > 7) { cout << "Invalid day.\n"; return; }

    string startTime, endTime, location;
    cout << "Start time (HH:MM): "; cin >> startTime;
    cout << "End time (HH:MM): "; cin >> endTime;
    cout << "Location: "; cin >> ws; getline(cin, location);

    try
    {
        TimeSlot slot(static_cast<Day>(dayNumber - 1), timeToMinutes(startTime), timeToMinutes(endTime), location);
        offering->getTimetable().addSlot(slot);
        cout << "Timetable slot added successfully.\n";
    }
    catch (const exception& e) { cout << "Failed to add timetable slot: " << e.what() << '\n'; }
}

void LMSConsole::runStudentMenu(Student& student)
{
    while (system.getCurrentUser())
    {
        student.getMenu();
        cout << "\nEnter choice: ";
        int choice;
        if (!readInteger(choice)) { if (cin.eof()) { system.logout(); return; } cout << "Invalid input.\n"; continue; }
        switch (choice)
        {
            case 1: viewAvailableOfferings(); break;
            case 2: enrolStudent(student); break;
            case 3: viewMyEnrolments(student); break;
            case 4: viewMyTimetable(student); break;
            case 0: system.logout(); cout << "Logged out successfully.\n"; break;
            default: cout << "Invalid menu choice.\n";
        }
    }
}

void LMSConsole::runLecturerMenu(Lecturer& lecturer)
{
    while (system.getCurrentUser())
    {
        lecturer.getMenu();
        cout << "\nEnter choice: ";
        int choice;
        if (!readInteger(choice)) { if (cin.eof()) { system.logout(); return; } cout << "Invalid input.\n"; continue; }
        switch (choice)
        {
            case 1: viewLecturerCourses(lecturer); break;
            case 2: viewOfferingEnrolments(lecturer); break;
            case 3:
                cout << "Attendance session creation/closing needs a writable AttendanceRegister accessor in CourseOffering.\n";
                break;
            case 4:
                cout << "Attendance marking/correction needs a writable AttendanceRegister accessor in CourseOffering.\n";
                break;
            case 5: viewAttendanceReport(lecturer); break;
            case 6: system.logout(); cout << "Logged out successfully.\n"; break;
            default: cout << "Invalid menu choice.\n";
        }
    }
}

void LMSConsole::runAdministratorMenu(Administrator& admin)
{
    while (system.getCurrentUser())
    {
        admin.getMenu();
        cout << "\nEnter choice: ";
        int choice;
        if (!readInteger(choice)) { if (cin.eof()) { system.logout(); return; } cout << "Invalid input.\n"; continue; }
        switch (choice)
        {
            case 1: viewAllUsers(); break;
            case 2: viewAllCourses(); break;
            case 3: viewAvailableOfferings(); break;
            case 4: createCourse(); break;
            case 5: createOffering(); break;
            case 6: addTimetableSlot(); break;
            case 0: system.logout(); cout << "Logged out successfully.\n"; break;
            default: cout << "Invalid menu choice.\n";
        }
    }
}

void LMSConsole::run()
{
    createTestData();

    while (true)
    {
        cout << "\n================================\n       UNIVERSITY LMS\n================================\n";
        cout << "1. Login\n0. Exit\n\nEnter choice: ";
        int choice;
        if (!readInteger(choice)) { if (cin.eof()) return; cout << "Invalid input. Enter a number.\n"; continue; }
        if (choice == 0) { cout << "Exiting University LMS.\n"; return; }
        if (choice != 1) { cout << "Invalid menu choice.\n"; continue; }

        string userId, password;
        cout << "User ID: "; cin >> userId;
        cout << "Password: "; cin >> password;
        User* loggedUser = system.login(userId, password);
        if (!loggedUser) { cout << "Invalid user ID or password.\n"; continue; }
        cout << "Login successful.\n";

        if (Student* student = dynamic_cast<Student*>(loggedUser))
            runStudentMenu(*student);
        else if (Lecturer* lecturer = dynamic_cast<Lecturer*>(loggedUser))
            runLecturerMenu(*lecturer);
        else if (Administrator* admin = dynamic_cast<Administrator*>(loggedUser))
            runAdministratorMenu(*admin);
        else
        {
            cout << "Unsupported user type.\n";
            system.logout();
        }
    }
}
