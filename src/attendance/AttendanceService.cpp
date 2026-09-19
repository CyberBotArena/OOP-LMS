#include "attendance/AttendanceService.h"
#include "attendance/AttendanceExceptions.h"
#include <stdexcept>
#include <string>

using namespace std;


// Constructor
AttendanceService::AttendanceService(
    AttendanceRegister* register_pointer)
{
    attendance_register = register_pointer;
    capture = nullptr;
    next_record_number = 1;
}


// Select attendance capture mechanism
void AttendanceService::setCapture(
    AttendanceCapture* new_capture)
{
    capture = new_capture;
}


// Run the selected attendance capture mechanism
void AttendanceService::runCapture(
    AttendanceSession& session)
{
    if (attendance_register == nullptr)
    {
        throw AttendanceException(
            "Attendance register is not available."
        );
    }

    if (capture == nullptr)
    {
        throw AttendanceException(
            "Attendance capture method is not selected."
        );
    }

    // Start capture
    capture->beginSession(session);

    string student_ID = capture->captureNext();

    while (!student_ID.empty())
    {
        string record_ID =
            "RECORD" + to_string(next_record_number);

        attendance_register->markPresent(
            record_ID,
            student_ID,
            session.getId(),
            capture->getMethodName()
        );

        next_record_number++;

        student_ID = capture->captureNext();
    }

    // Finish capture
    capture->endSession();
}
