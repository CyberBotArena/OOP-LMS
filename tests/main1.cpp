
#include <iostream>
#include <string>
#include <stdexcept>

#include "universitySystem/UniversitySystem.h"

#include "users/User.h"
#include "users/Student.h"
#include "users/Lecturer.h"
#include "users/Administrator.h"

#include "courses/Course.h"
#include "courses/LectureCourse.h"

#include "enrolment/CourseOffering.h"
#include "enrolment/EnrolmentService.h"

#include "scheduling/Timeslot.h"

using namespace std;


// --------------------------------------------------
// Display all available course offerings
// --------------------------------------------------
void viewAvailableOfferings(UniversitySystem& system)
{
    cout << "\n================================\n";
    cout << "       AVAILABLE OFFERINGS\n";
    cout << "================================\n";

    vector<CourseOffering*> offerings = system.getOfferings();

    if (offerings.empty())
    {
        cout << "No course offerings available.\n";
        return;
    }

    for (CourseOffering* offering : offerings)
    {
        cout << "\nOffering ID: "
             << offering->getOfferingId() << '\n';

        cout << "Semester: "
             << offering->getSemester() << '\n';

        cout << "Capacity: "
             << offering->getCapacity() << '\n';

        if (offering->getCourse() != nullptr)
        {
            cout << "Course: "
                 << offering->getCourse()->getCode()
                 << " - "
                 << offering->getCourse()->getTitle()
                 << '\n';
        }

        if (offering->getLecturer() != nullptr)
        {
            cout << "Lecturer: "
                 << offering->getLecturer()->getName()
                 << '\n';
        }

        cout << "------------------------------\n";
    }
}


string dayToString(Day day)
{
    switch (day)
    {
        case Day::Monday:    return "Monday";
        case Day::Tuesday:   return "Tuesday";
        case Day::Wednesday: return "Wednesday";
        case Day::Thursday:  return "Thursday";
        case Day::Friday:    return "Friday";
        case Day::Saturday:  return "Saturday";
        case Day::Sunday:    return "Sunday";
    }

    return "Unknown";
}


string minutesToTime(int minutes)
{
    int hours = minutes / 60;
    int mins = minutes % 60;

    string result;

    if (hours < 10)
        result += "0";

    result += to_string(hours);
    result += ":";

    if (mins < 10)
        result += "0";

    result += to_string(mins);

    return result;
}


int timeToMinutes(const string& time)
{
    // Expected format: HH:MM
    if (time.length() != 5 || time[2] != ':')
    {
        throw invalid_argument("Time must be in HH:MM format.");
    }

    for (int i : {0, 1, 3, 4})
    {
        if (time[i] < '0' || time[i] > '9')
        {
            throw invalid_argument("Time must contain digits.");
        }
    }

    int hours = stoi(time.substr(0, 2));
    int minutes = stoi(time.substr(3, 2));

    if (hours > 23 || minutes > 59)
    {
        throw invalid_argument("Invalid time value.");
    }

    return hours * 60 + minutes;
}


void addTimetableSlot(UniversitySystem& system)
{
    string offeringId;

    cout << "\n================================\n";
    cout << "     ADD TIMETABLE SLOT\n";
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

    cout << "\nSelect day:\n";
    cout << "1. Monday\n";
    cout << "2. Tuesday\n";
    cout << "3. Wednesday\n";
    cout << "4. Thursday\n";
    cout << "5. Friday\n";
    cout << "6. Saturday\n";
    cout << "7. Sunday\n";

    int dayNumber;

    cout << "Day: ";

    if (!(cin >> dayNumber))
    {
        cin.clear();
        cin.ignore(10000, '\n');

        cout << "Invalid day.\n";
        return;
    }

    if (dayNumber < 1 || dayNumber > 7)
    {
        cout << "Day must be between 1 and 7.\n";
        return;
    }

    // Day::Monday is 0, so subtract 1.
    Day day = static_cast<Day>(dayNumber - 1);

    string startTime;
    string endTime;
    string location;

    cout << "Start time (HH:MM): ";
    cin >> startTime;

    cout << "End time (HH:MM): ";
    cin >> endTime;

    cout << "Location: ";
    cin >> ws;
    getline(cin, location);

    try
    {
        int startMinutes = timeToMinutes(startTime);
        int endMinutes = timeToMinutes(endTime);

        TimeSlot slot(
            day,
            startMinutes,
            endMinutes,
            location
        );

        offering->getTimetable().addSlot(slot);

        cout << "\nTimetable slot added successfully!\n";

        cout << "Offering: "
             << offering->getOfferingId() << '\n';

        cout << "Day: "
             << dayToString(slot.getDay()) << '\n';

        cout << "Time: "
             << minutesToTime(slot.getStartMinutes())
             << " - "
             << minutesToTime(slot.getEndMinutes())
             << '\n';

        cout << "Location: "
             << slot.getLocation() << '\n';
    }
    catch (const exception& error)
    {
        cout << "\nFailed to add timetable slot: "
             << error.what() << '\n';
    }
}

// --------------------------------------------------
// Enrol a student in an offering
// --------------------------------------------------
void enrolStudent(
    UniversitySystem& system,
    Student& student,
    EnrolmentService& enrolmentService)
{
    string offeringId;

    cout << "\nEnter offering ID: ";
    cin >> offeringId;

    CourseOffering* offering =
        system.findOffering(offeringId);

    if (offering == nullptr)
    {
        cout << "Offering not found.\n";
        return;
    }

    try
    {
        enrolmentService.enrol(student, *offering);

        cout << "\nEnrolment successful!\n";

        cout << "Student: "
             << student.getName() << '\n';

        cout << "Offering: "
             << offering->getOfferingId() << '\n';
    }
    catch (const exception& error)
    {
        cout << "\nEnrolment failed: "
             << error.what() << '\n';
    }
}


// --------------------------------------------------
// Display offerings in which student is enrolled
// --------------------------------------------------
void viewMyEnrolments(
    UniversitySystem& system,
    Student& student)
{
    cout << "\n================================\n";
    cout << "          MY ENROLMENTS\n";
    cout << "================================\n";

    bool found = false;

    for (CourseOffering* offering : system.getOfferings())
    {
        if (student.isEnrolledIn(
                offering->getOfferingId()))
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

            cout << "------------------------------\n";
        }
    }

    if (!found)
    {
        cout << "You are not enrolled in any offerings.\n";
    }
}


void viewMyTimetable(Student& student)
{
    const vector<TimeSlot>& slots =
        student.getPersonalTimetable().getSlots();

    cout << "\n================================\n";
    cout << "          MY TIMETABLE\n";
    cout << "================================\n";

    if (slots.empty())
    {
        cout << "No timetable slots available.\n";
        return;
    }

    for (const TimeSlot& slot : slots)
    {
        cout << dayToString(slot.getDay())
             << " | "
             << minutesToTime(slot.getStartMinutes())
             << " - "
             << minutesToTime(slot.getEndMinutes())
             << " | "
             << slot.getLocation()
             << '\n';
    }
}


// --------------------------------------------------
// Student menu
// --------------------------------------------------
void studentMenu(
    UniversitySystem& system,
    Student& student,
    EnrolmentService& enrolmentService)
{
    int choice = -1;

    while (choice != 0)
    {
        cout << "\n================================\n";
        cout << "           STUDENT MENU\n";
        cout << "================================\n";

        cout << "Welcome, "
             << student.getName() << '\n';

        cout << "Student ID: "
             << student.getUserId() << "\n\n";

        cout << "1. View available offerings\n";
        cout << "2. Enrol in an offering\n";
        cout << "3. View my enrolments\n";
        cout << "4. View my timetable\n";
        cout << "0. Logout\n";

        cout << "\nEnter choice: ";

        if (!(cin >> choice))
        {
            if (cin.eof())
            {
                system.logout();
                return;
            }

            cin.clear();
            cin.ignore(10000, '\n');

            cout << "Invalid input. Enter a number.\n";
            continue;
        }

        switch (choice)
        {
            case 1:
                viewAvailableOfferings(system);
                break;

            case 2:
                enrolStudent(
                    system,
                    student,
                    enrolmentService
                );
                break;

            case 3:
                viewMyEnrolments(system, student);
                break;

            case 4:
                viewMyTimetable(student);
                break;

            case 0:
                system.logout();
                cout << "\nLogged out successfully.\n";
                break;

            default:
                cout << "Invalid menu choice.\n";
                break;
        }
    }
}


// --------------------------------------------------
// Create temporary test data
// --------------------------------------------------
void createTestData(UniversitySystem& system)
{
    // Create student
    Student* student = new Student(
        system.generateUserId(),
        "Kamal Perera",
        "kamal",
        "1234"
    );

    // Create lecturer
    Lecturer* lecturer = new Lecturer(
        system.generateUserId(),
        "Nimal Silva",
        "nimal",
        "1234"
    );

    // Create administrator
    Administrator* admin = new Administrator(
        system.generateUserId(),
        "Admin User",
        "admin",
        "admin123"
    );

    // UniversitySystem repositories own these pointers
    system.addUser(student);
    system.addUser(lecturer);
    system.addUser(admin);


    // Create course
    LectureCourse* course = new LectureCourse(
        "CS101",
        "Programming Fundamentals",
        3
    );

    system.addCourse(course);


    // Create course offering
    CourseOffering* offering = new CourseOffering(
        "OFF001",
        "Semester 1 - 2026",
        30
    );

    // Connect course and lecturer
    offering->setCourse(course);
    offering->setLecturer(lecturer);

    // Store offering in UniversitySystem
    system.addOffering(offering);


    cout << "\nTest data created successfully.\n";

    cout << "Student ID: "
         << student->getUserId() << '\n';

    cout << "Lecturer ID: "
         << lecturer->getUserId() << '\n';

    cout << "Administrator ID: "
         << admin->getUserId() << '\n';

    cout << "\nTest password for student: 1234\n";
}


void createCourse(UniversitySystem& system)
{
    string code;
    string title;
    int credits;

    cout << "\n================================\n";
    cout << "          CREATE COURSE\n";
    cout << "================================\n";

    cout << "Course code: ";
    cin >> code;

    // Prevent duplicate course codes
    if (system.findCourse(code) != nullptr)
    {
        cout << "A course with this code already exists.\n";
        return;
    }

    cout << "Course title: ";
    cin >> ws;
    getline(cin, title);

    cout << "Credits: ";

    if (!(cin >> credits))
    {
        cin.clear();
        cin.ignore(10000, '\n');

        cout << "Invalid credit value.\n";
        return;
    }

    if (credits <= 0)
    {
        cout << "Credits must be positive.\n";
        return;
    }

    try
    {
        LectureCourse* course = new LectureCourse(
            code,
            title,
            credits
        );

        system.addCourse(course);

        cout << "\nCourse created successfully!\n";
        cout << "Code: " << course->getCode() << '\n';
        cout << "Title: " << course->getTitle() << '\n';
    }
    catch (const exception& error)
    {
        cout << "Failed to create course: "
             << error.what() << '\n';
    }
}


void createOffering(UniversitySystem& system)
{
    string offeringId;
    string semester;
    string courseCode;
    string lecturerId;
    int capacity;

    cout << "\n================================\n";
    cout << "       CREATE OFFERING\n";
    cout << "================================\n";

    cout << "Offering ID: ";
    cin >> offeringId;

    if (system.findOffering(offeringId) != nullptr)
    {
        cout << "This offering ID already exists.\n";
        return;
    }

    cout << "Course code: ";
    cin >> courseCode;

    Course* course = system.findCourse(courseCode);

    if (course == nullptr)
    {
        cout << "Course not found. Create it first.\n";
        return;
    }

    cout << "Semester: ";
    cin >> ws;
    getline(cin, semester);

    cout << "Capacity: ";

    if (!(cin >> capacity))
    {
        cin.clear();
        cin.ignore(10000, '\n');

        cout << "Invalid capacity.\n";
        return;
    }

    if (capacity <= 0)
    {
        cout << "Capacity must be positive.\n";
        return;
    }

    cout << "Lecturer ID: ";
    cin >> lecturerId;

    User* user = system.findUser(lecturerId);

    Lecturer* lecturer = dynamic_cast<Lecturer*>(user);

    if (lecturer == nullptr)
    {
        cout << "Lecturer not found.\n";
        return;
    }

    try
    {
        CourseOffering* offering = new CourseOffering(
            offeringId,
            semester,
            capacity
        );

        offering->setCourse(course);
        offering->setLecturer(lecturer);

        system.addOffering(offering);

        cout << "\nOffering created successfully!\n";
        cout << "Offering ID: "
             << offering->getOfferingId() << '\n';
        cout << "Course: "
             << course->getTitle() << '\n';
        cout << "Lecturer: "
             << lecturer->getName() << '\n';
    }
    catch (const exception& error)
    {
        cout << "Failed to create offering: "
             << error.what() << '\n';
    }
}



void administratorMenu(
    UniversitySystem& system,
    Administrator& admin)
{
    int choice = -1;

    while (choice != 0)
    {
        cout << "\n================================\n";
        cout << "       ADMINISTRATOR MENU\n";
        cout << "================================\n";

        cout << "Welcome, " << admin.getName() << "\n\n";

        cout << "1. View all users\n";
        cout << "2. View all courses\n";
        cout << "3. View all offerings\n";
        cout << "4. Create course\n";
        cout << "5. Create offering\n";
        cout << "6. Add timetable slot to offering\n";
        cout << "0. Logout\n";

        cout << "\nEnter choice: ";

        if (!(cin >> choice))
        {
            if (cin.eof())
            {
                system.logout();
                return;
            }

            cin.clear();
            cin.ignore(10000, '\n');

            cout << "Invalid input.\n";
            continue;
        }

        switch (choice)
        {
            case 1:
            {
                cout << "\nALL USERS\n";

                for (User* user : system.getAllUsers())
                {
                    cout << user->getUserId()
                         << " | "
                         << user->getName()
                         << '\n';
                }

                break;
            }

            case 2:
            {
                cout << "\nALL COURSES\n";

                for (Course* course : system.getCourses())
                {
                    cout << course->getCode()
                         << " | "
                         << course->getTitle()
                         << " | "
                         << course->getCourseType()
                         << '\n';
                }

                break;
            }

            case 3:
                viewAvailableOfferings(system);
                break;

            case 4:
                createCourse(system);
                break;
            
            case 5:
                createOffering(system);
                break;

            case 6:
                addTimetableSlot(system);
                break;

            case 0:
                system.logout();
                cout << "Logged out.\n";
                break;

            default:
                cout << "Invalid choice.\n";
        }
    }
}

// --------------------------------------------------
// Main function
// --------------------------------------------------
int main()
{
    UniversitySystem system;

    EnrolmentService enrolmentService;

    // Temporary data for integration testing
    createTestData(system);

    int choice = -1;

    while (choice != 0)
    {
        cout << "\n================================\n";
        cout << "       UNIVERSITY LMS\n";
        cout << "================================\n";

        cout << "1. Login\n";
        cout << "0. Exit\n";

        cout << "\nEnter choice: ";

        if (!(cin >> choice))
        {
            if (cin.eof())
            {
                break;
            }

            cin.clear();
            cin.ignore(10000, '\n');

            cout << "Invalid input. Enter a number.\n";
            continue;
        }

        switch (choice)
        {
            case 1:
            {
                string userId;
                string password;

                cout << "\nUser ID: ";
                cin >> userId;

                cout << "Password: ";
                cin >> password;

                User* loggedUser =
                    system.login(userId, password);

                if (loggedUser == nullptr)
                {
                    cout << "\nInvalid user ID or password.\n";
                    break;
                }

                cout << "\nLogin successful!\n";

                if (loggedUser != nullptr)
                {
                    cout << "\nLogin successful!\n";
                
                    loggedUser->getMenu();
                }
                else
                {
                    cout << "\nInvalid user ID or password.\n";
                }

                break;
            }

            case 0:
                cout << "\nExiting University LMS.\n";
                break;

            default:
                cout << "Invalid menu choice.\n";
                break;
        }
    }

    return 0;
}