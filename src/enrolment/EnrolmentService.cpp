#include "enrolment/EnrolmentService.h"
#include "users/Student.h"
#include "enrolment/CourseOffering.h"
#include "enrolment/EnrolmentExceptions.h"

#include <stdexcept>

using namespace std;


void EnrolmentService::enrol(
    Student& student,
    CourseOffering& offering)
{
    // 1. Check whether student is already enrolled
    if (student.isEnrolledIn(
            offering.getOfferingId()))
    {
        throw AlreadyEnrolledException(
            "Student is already enrolled in this offering."
        );
    }


    // 2. Check course capacity
    if (offering.isFull())
    {
        throw CourseFullException(
            "Course offering is full."
        );
    }


    // 3. Check prerequisites
    //
    // We will add this after connecting the Course class.
    //
    // Example logic will eventually be:
    //
    // Course* course = offering.getCourse();
    //
    // for (const string& prerequisite :
    //      course->getPrerequisites())
    // {
    //     if (!student.hasCompletedCourse(prerequisite))
    //     {
    //         throw PrerequisiteNotMetException(...);
    //     }
    // }


    // 4. Check timetable clash
    if (student.getPersonalTimetable().clashWith(
            offering.getTimetable()))
    {
        throw ScheduleClashException(
            "Course timetable clashes with the student's timetable."
        );
    }


    // 5. Add student to the offering
    offering.addStudent(&student);


    // 6. Update student's enrolment information
    student.addEnrolledCourse(
        offering.getOfferingId()
    );


    // 7. Add offering timetable to student's timetable
    for (const TimeSlot& slot :
         offering.getTimetable().getSlots())
    {
        student.getPersonalTimetable().addSlot(slot);
    }
}


void EnrolmentService::drop(
    Student& student,
    CourseOffering& offering)
{
    // Check whether the student is enrolled
    if (!student.isEnrolledIn(
            offering.getOfferingId()))
    {
        throw runtime_error(
            "Student is not enrolled in this offering."
        );
    }


    // Remove student from the offering
    offering.removeStudent(
        to_string(student.getUserId())
    );


    // Remove offering from student's enrolment list
    student.removeEnrolledCourse(
        offering.getOfferingId()
    );
}