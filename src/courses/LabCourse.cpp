#include "courses/LabCourse.h"

#include <stdexcept>

LabCourse::LabCourse(const std::string& code,
                     const std::string& title,
                     int credits,
                     int labHoursPerWeek)
    : Course(code, title, credits),
      labHoursPerWeek(labHoursPerWeek)
{
    if (labHoursPerWeek <= 0) {
        throw std::invalid_argument("Lab hours per week must be greater than zero.");
    }
}

LabCourse::~LabCourse() = default;

int LabCourse::getLabHoursPerWeek() const {
    return labHoursPerWeek;
}

std::string LabCourse::getCourseType() const {
    return "Lab";
}

double LabCourse::gradeWeightMultiplier() const {
    return 1.2;
}
