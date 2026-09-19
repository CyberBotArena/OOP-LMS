#include "persistence/FileStorage.h"

#include "universitySystem/UniversitySystem.h"

#include "users/User.h"

#include "courses/Course.h"
#include "courses/LabCourse.h"
#include "courses/ProjectCourse.h"

#include "enrolment/CourseOffering.h"

#include "users/Student.h"
#include "users/Lecturer.h"
#include "users/Administrator.h"

#include "scheduling/Timeslot.h"
#include "scheduling/Timetable.h"

#include "attendance/AttendanceSession.h"
#include "attendance/AttendanceRecord.h"
#include "attendance/AttendanceRegister.h"

#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>

using namespace std;


// Constructor
FileStorage::FileStorage(
    const string& path)
    : base_path(path)
{
}


// Save data
void FileStorage::saveAll(UniversitySystem& system)
{

    cout << "NEW SAVEALL CODE IS RUNNING" << endl;
    string file_path =
        base_path + "/system_data.txt";

    ofstream output_file(file_path);

    if (!output_file.is_open())
    {
        throw runtime_error(
            "Could not open the file for saving."
        );
    }


    // =========================
    // Save users
    // =========================

    vector<User*> users =
        system.getAllUsers();

    output_file << "[USERS]\n";

    for (User* user : users)
    {
        if (user == nullptr)
        {
            continue;
        }

        output_file
            << user->getRole() << "|"
            << user->getUserId() << "|"
            << user->getName() << "|"
            << user->getUsername() << "|"
            << user->getPassword()
            << "\n";
    }


    // =========================
    // Save courses
    // =========================

    vector<Course*> courses =
        system.getCourses();

    output_file << "\n[COURSES]\n";

    for (Course* course : courses)
    {
        if (course == nullptr)
        {
            continue;
        }

        output_file
            << course->getCourseType() << "|"
            << course->getCode() << "|"
            << course->getTitle() << "|"
            << course->getCredits();


        // LabCourse specific data
        LabCourse* labCourse =
            dynamic_cast<LabCourse*>(course);

        if (labCourse != nullptr)
        {
            output_file
                << "|"
                << labCourse->getLabHoursPerWeek();
        }


        // ProjectCourse specific data
        ProjectCourse* projectCourse =
            dynamic_cast<ProjectCourse*>(course);

        if (projectCourse != nullptr)
        {
            output_file
                << "|"
                << projectCourse->getGroupSize();
        }


        // Save prerequisites
        output_file << "|";

        const vector<string>& prerequisites =
            course->getPrerequisites();

        for (size_t i = 0;
             i < prerequisites.size();
             i++)
        {
            output_file
                << prerequisites[i];

            if (i + 1 < prerequisites.size())
            {
                output_file << ",";
            }
        }

        output_file << "\n";
    }

    // =====================================
// Save offerings
// =====================================

vector<CourseOffering*> offerings =
    system.getOfferings();

output_file << "\n[OFFERINGS]\n";

for (CourseOffering* offering : offerings)
{
    if (offering == nullptr)
        continue;

    output_file
        << offering->getOfferingId() << "|"
        << offering->getSemester() << "|"
        << offering->getCapacity() << "|";

    if (offering->getCourse())
        output_file << offering->getCourse()->getCode();

    output_file << "|";

    if (offering->getLecturer())
        output_file << offering->getLecturer()->getUserId();

    output_file << "\n";
}


// =====================================
// Save offering timetables
// =====================================

output_file << "\n[TIMETABLES]\n";

for (CourseOffering* offering : offerings)
{
    if (offering == nullptr)
        continue;

    const vector<TimeSlot>& slots =
        offering->getTimetable().getSlots();

    for (const TimeSlot& slot : slots)
    {
        output_file
            << offering->getOfferingId() << "|"
            << static_cast<int>(slot.getDay()) << "|"
            << slot.getStartMinutes() << "|"
            << slot.getEndMinutes() << "|"
            << slot.getLocation()
            << "\n";
    }
}


// =====================================
// Save enrolments
// =====================================

output_file << "\n[ENROLMENTS]\n";

for (CourseOffering* offering : offerings)
{
    if (offering == nullptr)
        continue;

    const vector<Student*>& students =
        offering->getEnrolledStudents();

    for (Student* student : students)
    {
        if (student != nullptr)
        {
            output_file
                << offering->getOfferingId() << "|"
                << student->getUserId()
                << "\n";
        }
    }
}


// =====================================
// Save attendance sessions
// =====================================

output_file << "\n[ATTENDANCE_SESSIONS]\n";

for (CourseOffering* offering : offerings)
{
    if (offering == nullptr)
        continue;

    const AttendanceRegister& attendance =
        offering->getAttendanceRegister();

    const vector<AttendanceSession>& sessions =
        attendance.getSessions();

    for (const AttendanceSession& session : sessions)
    {
        const TimeSlot& slot =
            session.getSlot();

        output_file
            << session.getId() << "|"
            << session.getOfferingId() << "|"
            << session.getLecturerId() << "|"
            << static_cast<int>(slot.getDay()) << "|"
            << slot.getStartMinutes() << "|"
            << slot.getEndMinutes() << "|"
            << slot.getLocation() << "|"
            << (session.isOpen() ? 1 : 0)
            << "\n";
    }
}


// =====================================
// Save attendance records
// =====================================

output_file << "\n[ATTENDANCE_RECORDS]\n";

for (CourseOffering* offering : offerings)
{
    if (offering == nullptr)
        continue;

    const AttendanceRegister& attendance =
        offering->getAttendanceRegister();

    const vector<AttendanceRecord>& records =
        attendance.getRecords();

    for (const AttendanceRecord& record : records)
    {
        output_file
            << record.getRecordID() << "|"
            << record.getStudentID() << "|"
            << record.getSessionID() << "|"
            << static_cast<int>(record.getStatus()) << "|"
            << record.getCaptureMethod() << "|"
            << record.getCorrectionOf() << "|"
            << record.getActingLecturerID() << "|"
            << record.getReason()
            << "\n";
    }
}


    output_file.close();
}


// Load data
void FileStorage::loadAll(UniversitySystem& system)
{
    (void)system;
    string file_path =
        base_path + "/system_data.txt";

    ifstream input_file(file_path);

    if (!input_file.is_open())
    {
        throw runtime_error(
            "Could not open the file for loading."
        );
    }

    string line;

    while (getline(input_file, line))
    {
        // Reading each stored line
    }

    input_file.close();
}


// Return read-only base path
const string& FileStorage::getBasePath() const
{
    return base_path;
}