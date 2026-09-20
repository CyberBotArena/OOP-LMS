#include "persistence/FileStorage.h"
#include "universitySystem/UniversitySystem.h"

#include "users/User.h"
#include "users/Student.h"
#include "users/Lecturer.h"
#include "users/Administrator.h"

#include "courses/Course.h"
#include "courses/LectureCourse.h"
#include "courses/LabCourse.h"
#include "courses/ProjectCourse.h"

#include "enrolment/CourseOffering.h"

#include "scheduling/Timeslot.h"
#include "scheduling/Timetable.h"

#include "attendance/AttendanceSession.h"
#include "attendance/AttendanceRecord.h"
#include "attendance/AttendanceRegister.h"

#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <stdexcept>
#include <chrono>

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

    cout << "Saved all successfully." << endl;
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

static vector<string> splitFields(
    const string& line,
    char delimiter = '|'
)
{
    vector<string> fields;
    string field;

    istringstream stream(line);

    while (getline(stream, field, delimiter))
    {
        fields.push_back(field);
    }

    if (!line.empty() && line.back() == delimiter)
    {
        fields.push_back("");
    }

    return fields;
}

// Load data

void FileStorage::loadAll(UniversitySystem& system)
{
    string file_path =
        base_path + "/system_data.txt";

    ifstream input_file(file_path);

    if (!input_file.is_open())
    {
        throw runtime_error(
            "Could not open the file for loading."
        );
    }

    // Read the saved sections.

    map<string, vector<vector<string>>> sections;

    string section;
    string line;

    while (getline(input_file, line))
    {
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        if (line.empty())
        {
            continue;
        }

        if (line.front() == '[' && line.back() == ']')
        {
            section = line;
            continue;
        }

        if (section.empty())
        {
            throw runtime_error(
                "Saved data contains a record outside a section."
            );
        }

        sections[section].push_back(
            splitFields(line)
        );
    }

    input_file.close();

    // Historical attendance cannot be reconstructed
    // completely through the current UML interfaces.
    // Reject it instead of silently losing data.

    if (!sections["[ATTENDANCE_SESSIONS]"].empty() ||
        !sections["[ATTENDANCE_RECORDS]"].empty())
    {
        throw runtime_error(
            "Historical attendance restoration requires "
            "an interface supported by the UML."
        );
    }

    // =========================================
    // 1. Load users
    // =========================================

    for (const auto& fields : sections["[USERS]"])
    {
        if (fields.size() != 5)
        {
            throw runtime_error(
                "Invalid saved user record."
            );
        }

        const string& role = fields[0];
        const string& id = fields[1];
        const string& name = fields[2];
        const string& username = fields[3];
        const string& password = fields[4];

        if (system.findUser(id) != nullptr)
        {
            throw runtime_error(
                "Duplicate user ID: " + id
            );
        }

        User* user = nullptr;

        if (role == "Student")
        {
            user = new Student(
                id, name, username, password
            );
        }
        else if (role == "Lecturer")
        {
            user = new Lecturer(
                id, name, username, password
            );
        }
        else if (role == "Administrator")
        {
            user = new Administrator(
                id, name, username, password
            );
        }
        else
        {
            throw runtime_error(
                "Unknown user role: " + role
            );
        }

        system.addUser(user);
    }

    // =========================================
    // 2. Load courses
    // =========================================

    for (const auto& fields : sections["[COURSES]"])
    {
        if (fields.size() < 5)
        {
            throw runtime_error(
                "Invalid saved course record."
            );
        }

        const string& type = fields[0];
        const string& code = fields[1];
        const string& title = fields[2];

        int credits = stoi(fields[3]);

        if (system.findCourse(code) != nullptr)
        {
            throw runtime_error(
                "Duplicate course code: " + code
            );
        }

        Course* course = nullptr;

        size_t prerequisiteIndex = 4;

        if (type == "Lecture")
        {
            course = new LectureCourse(
                code, title, credits
            );
        }
        else if (type == "Lab")
        {
            if (fields.size() < 6)
            {
                throw runtime_error(
                    "Invalid LabCourse record."
                );
            }

            int labHours = stoi(fields[4]);

            course = new LabCourse(
                code, title, credits, labHours
            );

            prerequisiteIndex = 5;
        }
        else if (type == "Project")
        {
            if (fields.size() < 6)
            {
                throw runtime_error(
                    "Invalid ProjectCourse record."
                );
            }

            int groupSize = stoi(fields[4]);

            course = new ProjectCourse(
                code, title, credits, groupSize
            );

            prerequisiteIndex = 5;
        }
        else
        {
            throw runtime_error(
                "Unknown course type: " + type
            );
        }

        // Restore prerequisites using the
        // existing Course method.

        vector<string> prerequisites =
            splitFields(fields[prerequisiteIndex], ',');

        for (const string& prerequisite : prerequisites)
        {
            if (!prerequisite.empty())
            {
                course->addPrerequisite(prerequisite);
            }
        }

        system.addCourse(course);
    }

    // =========================================
    // 3. Load offerings
    // =========================================

    for (const auto& fields : sections["[OFFERINGS]"])
    {
        if (fields.size() != 5)
        {
            throw runtime_error(
                "Invalid saved offering record."
            );
        }

        const string& offeringId = fields[0];
        const string& semester = fields[1];

        int capacity = stoi(fields[2]);

        const string& courseCode = fields[3];
        const string& lecturerId = fields[4];

        if (system.findOffering(offeringId) != nullptr)
        {
            throw runtime_error(
                "Duplicate offering ID: " + offeringId
            );
        }

        Course* course =
            system.findCourse(courseCode);

        Lecturer* lecturer =
            dynamic_cast<Lecturer*>(
                system.findUser(lecturerId)
            );

        if (course == nullptr || lecturer == nullptr)
        {
            throw runtime_error(
                "Missing course or lecturer for offering: "
                + offeringId
            );
        }

        CourseOffering* offering =
            new CourseOffering(
                offeringId,
                semester,
                capacity
            );

        offering->setCourse(course);
        offering->setLecturer(lecturer);

        lecturer->assignCourse(courseCode);

        system.addOffering(offering);
    }

    // =========================================
    // 4. Load timetables
    // =========================================

    for (const auto& fields : sections["[TIMETABLES]"])
    {
        if (fields.size() != 5)
        {
            throw runtime_error(
                "Invalid saved timetable record."
            );
        }

        CourseOffering* offering =
            system.findOffering(fields[0]);

        if (offering == nullptr)
        {
            throw runtime_error(
                "Offering not found for timetable."
            );
        }

        int dayValue = stoi(fields[1]);

        if (dayValue < 0 || dayValue > 4)
        {
            throw runtime_error(
                "Invalid saved timetable day."
            );
        }

        TimeSlot slot(
            static_cast<Day>(dayValue),
            stoi(fields[2]),
            stoi(fields[3]),
            fields[4]
        );

        Timetable timetable =
            offering->getTimetable();

        timetable.addSlot(slot);

        offering->setTimetable(timetable);
    }

    // =========================================
    // 5. Load enrolments
    // =========================================

    for (const auto& fields : sections["[ENROLMENTS]"])
    {
        if (fields.size() != 2)
        {
            throw runtime_error(
                "Invalid saved enrolment record."
            );
        }

        CourseOffering* offering =
            system.findOffering(fields[0]);

        Student* student =
            dynamic_cast<Student*>(
                system.findUser(fields[1])
            );

        if (offering == nullptr || student == nullptr)
        {
            throw runtime_error(
                "Missing offering or student for enrolment."
            );
        }

        if (offering->getCourse() == nullptr)
        {
            throw runtime_error(
                "Enrolment offering has no course."
            );
        }

        offering->addStudent(student);

        student->addEnrolledCourse(
            offering->getCourse()->getCode()
        );

        // Rebuild the student's personal timetable.

        for (const TimeSlot& slot :
             offering->getTimetable().getSlots())
        {
            student->getPersonalTimetable().addSlot(slot);
        }
    }
}