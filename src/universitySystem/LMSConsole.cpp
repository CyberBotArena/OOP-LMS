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
#include "attendance/AttendanceSession.h"
#include "attendance/AttendanceRegister.h"
#include "attendance/AttendanceService.h"
#include "capture/FileReplayCapture.h"
#include "capture/CardTapCapture.h"

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
    cout << "\n================================\n";
    cout << "          MY COURSES\n";
    cout << "================================\n";

    bool found = false;

    for (CourseOffering* offering : system.getOfferings())
    {
        if (offering->getLecturer() == &lecturer)
        {
            found = true;

            cout << "\nOffering ID: "
                 << offering->getOfferingId() << '\n';

            if (offering->getCourse() != nullptr)
            {
                cout << "Course: "
                     << offering->getCourse()->getCode()
                     << " - "
                     << offering->getCourse()->getTitle()
                     << '\n';
            }

            cout << "Semester: "
                 << offering->getSemester() << '\n';

            cout << "Capacity: "
                 << offering->getCapacity() << '\n';

            cout << "------------------------------\n";
        }
    }

    if (!found)
    {
        cout << "No courses are currently assigned to you.\n";
    }
}

void LMSConsole::viewEnrolmentList(const Lecturer& lecturer) const
{
    string offeringId;

    cout << "\n================================\n";
    cout << "       VIEW ENROLMENT LIST\n";
    cout << "================================\n";

    cout << "Enter offering ID: ";
    cin >> offeringId;

    CourseOffering* offering =
        system.findOffering(offeringId);

    if (offering == nullptr)
    {
        cout << "Offering not found.\n";
        return;
    }

    // Make sure this offering belongs to the logged-in lecturer
    if (offering->getLecturer() != &lecturer)
    {
        cout << "This offering is not assigned to you.\n";
        return;
    }

    cout << "\nOffering: "
         << offering->getOfferingId()
         << '\n';

    if (offering->getCourse() != nullptr)
    {
        cout << "Course: "
             << offering->getCourse()->getCode()
             << " - "
             << offering->getCourse()->getTitle()
             << '\n';
    }

    cout << "\nEnrolled Students\n";
    cout << "------------------------------\n";

    const vector<Student*>& students =
        offering->getEnrolledStudents();

    if (students.empty())
    {
        cout << "No students are enrolled in this offering.\n";
        return;
    }

    for (Student* student : students)
    {
        if (student != nullptr)
        {
            cout << "Student ID: "
                 << student->getUserId()
                 << " | Name: "
                 << student->getName()
                 << '\n';
        }
    }
}

/*void LMSConsole::viewOfferingEnrolments(const Lecturer& lecturer) const
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
}*/


void LMSConsole::dropCourse(Student& student)
{
    string offeringId;

    cout << "\n===== DROP COURSE =====\n";

    // Show the student's existing enrolments
    viewMyEnrolments(student);

    cout << "\nEnter offering ID to drop: ";
    cin >> offeringId;

    CourseOffering* offering =
        system.findOffering(offeringId);

    if (offering == nullptr)
    {
        cout << "Offering not found.\n";
        return;
    }

    if (!student.isEnrolledIn(offeringId))
    {
        cout << "You are not enrolled in this offering.\n";
        return;
    }

    try
    {
        // Use your existing enrolment service
        enrolmentService.drop(student, *offering);

        cout << "Course dropped successfully.\n";
    }
    catch (const exception& e)
    {
        cout << "Failed to drop course: "
             << e.what() << '\n';
    }
}

void LMSConsole::manageAttendanceSession(Lecturer& lecturer)
{
    string offeringId;

    cout << "\n================================\n";
    cout << "       ATTENDANCE SESSION\n";
    cout << "================================\n";

    cout << "Offering ID: ";
    cin >> offeringId;

    CourseOffering* offering =
        system.findOffering(offeringId);

    if (offering == nullptr)
    {
        cout << "Offering not found.\n";
        return;
    }

    if (offering->getLecturer() != &lecturer)
    {
        cout << "This offering is not assigned to you.\n";
        return;
    }

    cout << "\n1. Open attendance session\n";
    cout << "2. Close attendance session\n";
    cout << "0. Back\n";
    cout << "Enter choice: ";

    int choice;

    if (!readInteger(choice))
    {
        cout << "Invalid input.\n";
        return;
    }

    AttendanceRegister& attendance = offering->getAttendanceRegister();

    try
    {
        if (choice == 1)
        {
            string sessionId;
            int duration;

            cout << "Session ID: ";
            cin >> sessionId;

            const vector<TimeSlot>& slots =
                offering->getTimetable().getSlots();

            if (slots.empty())
            {
                cout << "No timetable slot exists for this offering.\n";
                return;
            }

            // For now use the first timetable slot
            const TimeSlot& slot = slots[0];

            cout << "Duration in minutes: ";

            if (!readInteger(duration) || duration <= 0)
            {
                cout << "Invalid duration.\n";
                return;
            }

            AttendanceSession session(
                sessionId,
                offering->getOfferingId(),
                lecturer.getUserId(),
                slot,
                duration
            );

            // REUSE YOUR EXISTING FUNCTION
            attendance.addSession(session);

            cout << "Attendance session opened successfully.\n";
        }
        else if (choice == 2)
        {
            string sessionId;

            cout << "Session ID: ";
            cin >> sessionId;

            // REUSE YOUR EXISTING FUNCTION
            attendance.closeSession(sessionId);

            cout << "Attendance session closed successfully.\n";
        }

        else if (choice != 0)
        {
            cout << "Invalid choice.\n";
        }
    }
    catch (const exception& e)
    {
        // DuplicateRecordException,
        // AttendanceNotFoundException, etc.
        // are handled here.
        cout << "Attendance operation failed: "
             << e.what() << '\n';
    }
}

void LMSConsole::manageAttendanceRecords(Lecturer& lecturer)
{
    string offeringId;
    cout << "\nOffering ID: ";
    cin >> offeringId;
    CourseOffering* offering = system.findOffering(offeringId);
    if (offering == nullptr || offering->getLecturer() != &lecturer)
    {
        cout << "Offering not found or not assigned to you.\n";
        return;
    }

    AttendanceRegister& attendance = offering->getAttendanceRegister();
    cout << "\n1. Mark attendance using file replay\n";
    cout << "2. Mark attendance using card tap\n";
    cout << "3. Correct an attendance record\n";
    cout << "0. Back\nEnter choice: ";
    int choice;
    if (!readInteger(choice))
    {
        cout << "Invalid input.\n";
        return;
    }
    if (choice == 0) return;

    try
    {
        if (choice == 1)
        {
            string sessionId, filePath;
            cout << "Session ID: ";
            cin >> sessionId;
            AttendanceSession* session = attendance.findSession(sessionId);
            if (session == nullptr || !session->isOpen())
            {
                cout << "Session not found, closed, or expired.\n";
                return;
            }
            cout << "Attendance file path: ";
            cin >> ws;
            getline(cin, filePath);
            FileReplayCapture capture(filePath);
            AttendanceService service(&attendance);
            service.setCapture(&capture);
            service.runCapture(*session);
            cout << "File replay completed.\n";
        }
        
        else if (choice == 2)
        {
            string sessionId;
        
            cout << "Session ID: ";
            cin >> sessionId;
        
            AttendanceSession* session =
                attendance.findSession(sessionId);
        
            if (session == nullptr || !session->isOpen())
            {
                cout << "Session not found, closed, or expired.\n";
                return;
            }
        
            CardTapCapture capture;
        
            AttendanceService service(&attendance);
            service.setCapture(&capture);
        
            cout << "\nCard-tap attendance started.\n";
            cout << "Enter each student's ID when prompted.\n";
            cout << "Enter END to finish.\n\n";
            cin.ignore(10000, '\n');
        
            service.runCapture(*session);
        
            cout << "Card-tap attendance completed.\n";
        }

        else if (choice == 3)
        {
            const vector<AttendanceRecord>& records = attendance.getRecords();
            if (records.empty())
            {
                cout << "No attendance records to correct.\n";
                return;
            }
            cout << "\nExisting records:\n";
            for (const AttendanceRecord& record : records)
            {
                cout << record.getRecordID() << " | Student: "
                     << record.getStudentID() << " | Session: "
                     << record.getSessionID() << '\n';
            }
            string originalId, newId, reason;
            int statusChoice;
            cout << "Original record ID: ";
            cin >> originalId;
            const AttendanceRecord* original = attendance.findRecord(originalId);
            if (original == nullptr)
            {
                cout << "Original record not found.\n";
                return;
            }
            const AttendanceSession* session = attendance.findSession(original->getSessionID());
            if (session == nullptr || !session->isOpen())
            {
                cout << "The original session is closed or expired.\n";
                return;
            }
            cout << "New correction record ID: ";
            cin >> newId;
            cout << "1. Present  2. Absent  3. Late  4. Excused\nStatus: ";
            if (!readInteger(statusChoice) || statusChoice < 1 || statusChoice > 4)
            {
                cout << "Invalid status.\n";
                return;
            }
            cout << "Correction reason: ";
            cin >> ws;
            getline(cin, reason);
            AttendanceStatus status = AttendanceStatus::Present;
            switch (statusChoice)
            {
                case 2: status = AttendanceStatus::Absent; break;
                case 3: status = AttendanceStatus::Late; break;
                case 4: status = AttendanceStatus::Excused; break;
                default: break;
            }
            attendance.appendCorrection(newId, originalId, status,
                                        lecturer.getUserId(), reason);
            cout << "Correction recorded successfully.\n";
        }
        else
        {
            cout << "Invalid choice.\n";
        }
    }
    catch (const exception& e)
    {
        cout << "Attendance operation failed: " << e.what() << '\n';
    }
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

void LMSConsole::createUser()
{
    string userId;
    string name;
    string username;
    string password;
    int role;

    cout << "\n===== Create User =====\n";

    cout << "Enter User ID: ";
    cin >> userId;

    if (userId.empty())
    {
        cout << "User ID cannot be empty.\n";
        return;
    }

    if (system.findUser(userId) != nullptr)
    {
        cout << "User ID already exists.\n";
        return;
    }

    cout << "Enter Name: ";
    cin >> ws;
    getline(cin, name);

    cout << "Enter Username: ";
    cin >> username;

    cout << "Enter Password: ";
    cin >> password;

    cout << "\nSelect Role:\n";
    cout << "1. Student\n";
    cout << "2. Lecturer\n";
    cout << "3. Administrator\n";
    cout << "Choice: ";
    cin >> role;

    User* newUser = nullptr;

    switch (role)
    {
        case 1:
            newUser = new Student(
                userId, name, username, password
            );
            break;

        case 2:
            newUser = new Lecturer(
                userId, name, username, password
            );
            break;

        case 3:
            newUser = new Administrator(
                userId, name, username, password
            );
            break;

        default:
            cout << "Invalid role.\n";
            return;
    }

    try
    {
        system.addUser(newUser);
        cout << "User created successfully.\n";
    }
    catch (const exception& e)
    {
        // Use this delete only if addUser() does not
        // take ownership when insertion fails.
        delete newUser;

        cout << "Failed to create user: "
             << e.what() << endl;
    }
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
            case 5: dropCourse(student); break;
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
            case 1:
            viewLecturerCourses(lecturer);
            break;            
            case 2: viewEnrolmentList(lecturer); break;
            case 3:
            manageAttendanceSession(lecturer);
            break;
            case 4: manageAttendanceRecords(lecturer); break;
            case 5: viewAttendanceReport(lecturer); break;
            case 0: system.logout(); cout << "Logged out successfully.\n"; break;
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
            case 7: createUser(); break;
            case 0: system.logout(); cout << "Logged out successfully.\n"; break;
            default: cout << "Invalid menu choice.\n";
        }
    }
}

void LMSConsole::run()
{
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
