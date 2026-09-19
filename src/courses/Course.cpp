#include "courses/Course.h"

#include <algorithm>
#include <stdexcept>

Course::Course(const std::string& code,
               const std::string& title,
               int credits)
    : code(code), title(title), credits(credits)
{
    if (code.empty()) {
        throw std::invalid_argument("Course code cannot be empty.");
    }
    if (title.empty()) {
        throw std::invalid_argument("Course title cannot be empty.");
    }
    if (credits <= 0) {
        throw std::invalid_argument("Course credits must be greater than zero.");
    }
}

Course::~Course() = default;

const std::string& Course::getCode() const { return code; }
const std::string& Course::getTitle() const { return title; }
int Course::getCredits() const { return credits; }

const std::vector<std::string>& Course::getPrerequisites() const {
    return prerequisites;
}

void Course::addPrerequisite(const std::string& courseCode) {
    if (courseCode.empty()) {
        throw std::invalid_argument("Prerequisite course code cannot be empty.");
    }

    if (!requiresPrerequisite(courseCode)) {
        prerequisites.push_back(courseCode);
    }
}

bool Course::requiresPrerequisite(const std::string& courseCode) const {
    return std::find(prerequisites.begin(), prerequisites.end(), courseCode)
           != prerequisites.end();
}

std::ostream& operator<<(std::ostream& os, const Course& course) {
    // These two calls are polymorphic. The derived implementation is chosen
    // at runtime even when the object is accessed through Course&.
    os << "[" << course.getCourseType() << "] "
       << course.getCode() << " - "
       << course.getTitle() << " ("
       << course.getCredits() << " credits, weight "
       << course.gradeWeightMultiplier() << ")";

    return os;
}
