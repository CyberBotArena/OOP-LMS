#ifndef ATTENDANCE_SERVICE_H
#define ATTENDANCE_SERVICE_H

#include "AttendanceRegister.h"
#include "../capture/AttendanceCapture.h"

class AttendanceService {
private:
    AttendanceCapture* capture;
    AttendanceRegister* attendance_register;
    int next_record_number;

public:
    AttendanceService(AttendanceRegister* register_pointer);

    void setCapture(AttendanceCapture* new_capture);

    void runCapture(AttendanceSession& session);
};

#endif