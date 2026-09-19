#include "courses/ProjectCourse.h"

ProjectCourse::ProjectCourse(const std::string& code, const std::string& title, int credits, int groupSize)
    : Course(code, title, credits), groupSize(groupSize) {}

ProjectCourse::~ProjectCourse() {}

int ProjectCourse::getGroupSize() const {
    return groupSize;
}

std::string ProjectCourse::getCourseType() const {
    return "Project";
}

double ProjectCourse::gradeWeightMultiplier() const {
    return 1.5; // project outcome weighted most heavily
}
