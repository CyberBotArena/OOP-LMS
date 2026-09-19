#include "courses/LectureCourse.h"

LectureCourse::LectureCourse(const std::string& code, const std::string& title, int credits)
    : Course(code, title, credits) {}

LectureCourse::~LectureCourse() {}

std::string LectureCourse::getCourseType() const {
    return "Lecture";
}

double LectureCourse::gradeWeightMultiplier() const {
    return 1.0; // baseline weighting
}
