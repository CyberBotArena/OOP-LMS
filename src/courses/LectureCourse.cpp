#include "LectureCourse.h"

LectureCourse::LectureCourse(const std::string& code, const std::string& title, int credits, int capacity)
    : Course(code, title, credits, capacity) {}

LectureCourse::~LectureCourse() {}

std::string LectureCourse::getCourseType() const {
    return "Lecture";
}

double LectureCourse::gradeWeightMultiplier() const {
    return 1.0; // baseline weighting
}
