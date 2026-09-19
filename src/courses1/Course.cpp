#include "courses/Course.h"
#include <algorithm>

Course::Course( const std::string& code, 
                const std::string& title, 
                int credits )
                //int capacity)
    : code(code), 
      title(title), 
      credits(credits) 
      //capacity(capacity)
      //enrolledCount(0) 
      {}

Course::~Course() {}

const std::string& Course::getCode() const { return code; }
const std::string& Course::getTitle() const { return title; }
int Course::getCredits() const { return credits; }
//int Course::getCapacity() const { return capacity; }
//int Course::getEnrolledCount() const { return enrolledCount; }
//const std::string& Course::getLecturerUsername() const { return lecturerUsername; }
const std::vector<std::string>& Course::getPrerequisites() const { return prerequisites; }

/*bool Course::hasCapacity() const {
    return enrolledCount < capacity;
}*/

/*void Course::incrementEnrolled() {
    if (hasCapacity()) enrolledCount++;
}*/

/*void Course::decrementEnrolled() {
    if (enrolledCount > 0) enrolledCount--;
}*/

//void Course::setLecturer(const std::string& newLecturerUsername) {
//    lecturerUsername = newLecturerUsername;
//}

void Course::addPrerequisite(const std::string& courseCode) {
    if (std::find(prerequisites.begin(), prerequisites.end(), courseCode) == prerequisites.end()) {
        prerequisites.push_back(courseCode);
    }
}

bool Course::requiresPrerequisite(const std::string& courseCode) const {
    return std::find(prerequisites.begin(), prerequisites.end(), courseCode) != prerequisites.end();
}

/*std::ostream& operator<<(std::ostream& os, const Course& course) {
    os << "[" << course.getCourseType() << "] "
       << course.code << " - " << course.title
       << " (" << course.credits << " credits, "
       << course.enrolledCount << "/" << course.capacity << " enrolled)";
    return os;
}
*/