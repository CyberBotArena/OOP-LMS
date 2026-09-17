#include <exception>
#include <iostream>

#include "AttendanceService.h"
#include "../capture/FileReplayCapture.h"

using namespace std;

int main() {
    try {
        // 1. Create the time slot
        TimeSlot slot(
            Day::Monday,
            9 * 60,       // 9:00 AM
            11 * 60,      // 11:00 AM
            "Room A"
        );

        // 2. Create an attendance session
        AttendanceSession session(
            "SESSION001",
            "OFFERING001",
            "LECTURER001",
            slot,
            60
        );

        // 3. Create the register and store the session
        AttendanceRegister attendance_register;
        attendance_register.addSession(session);

        // Get the session stored inside AttendanceRegister
        AttendanceSession* stored_session =
            attendance_register.findSession("SESSION001");

        if (stored_session == nullptr) {
            cout << "Test failed: session was not stored.\n";
            return 1;
        }

        // 4. Create the file capture object
        FileReplayCapture file_capture("../../src/attendance.txt");

        // 5. Connect the service to the register and capture method
        AttendanceService service(&attendance_register);
        service.setCapture(&file_capture);

        // 6. Read the IDs and create attendance records
        service.runCapture(*stored_session);

        // 7. Check the result
        int record_count =
            (int)attendance_register.getRecords().size();

        cout << "Number of records created: "
             << record_count << '\n';

        if (record_count == 3) {
            cout << "Test passed.\n";
        }
        else {
            cout << "Test failed. Expected 3 records.\n";
        }

        // 8. Display the created records
        cout << "\nAttendance records:\n";

        for (const AttendanceRecord& record :
             attendance_register.getRecords()) {

            cout << record.getRecordID()
                 << " | "
                 << record.getStudentID()
                 << " | "
                 << record.getSessionID()
                 << " | "
                 << record.getCaptureMethod()
                 << '\n';
        }
    }
    catch (const exception& error) {
        cout << "Test failed with error: "
             << error.what() << '\n';

        return 1;
    }

    return 0;
}
