#ifndef ENROLMENT_SERVICE_H
#define ENROLMENT_SERVICE_H

class Student;
class CourseOffering;

class EnrolmentService {
public:
    void enrol(
        Student& student,
        CourseOffering& offering
    );

    void drop(
        Student& student,
        CourseOffering& offering
    );
};

#endif