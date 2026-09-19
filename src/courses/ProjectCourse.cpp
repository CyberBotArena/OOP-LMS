#include "courses/ProjectCourse.h"

#include <stdexcept>

ProjectCourse::ProjectCourse(const std::string& code,
                             const std::string& title,
                             int credits,
                             int groupSize)
    : Course(code, title, credits),
      groupSize(groupSize)
{
    if (groupSize <= 0) {
        throw std::invalid_argument("Project group size must be greater than zero.");
    }
}

ProjectCourse::~ProjectCourse() = default;

int ProjectCourse::getGroupSize() const {
    return groupSize;
}

std::string ProjectCourse::getCourseType() const {
    return "Project";
}

double ProjectCourse::gradeWeightMultiplier() const {
    return 1.5;
}
