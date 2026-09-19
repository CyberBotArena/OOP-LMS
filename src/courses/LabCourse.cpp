#include "courses/LabCourse.h"

LabCourse::LabCourse(const std::string& code, const std::string& title, int credits, int labHoursPerWeek)
    : Course(code, title, credits), labHoursPerWeek(labHoursPerWeek) {}

LabCourse::~LabCourse() {}

int LabCourse::getLabHoursPerWeek() const {
    return labHoursPerWeek;
}

std::string LabCourse::getCourseType() const {
    return "Lab";
}

double LabCourse::gradeWeightMultiplier() const {
    return 1.2; // practical work weighted slightly higher
}
