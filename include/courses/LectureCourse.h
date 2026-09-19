#ifndef LECTURECOURSE_H
#define LECTURECOURSE_H

#include "courses/Course.h"

// Standard lecture-based course: straightforward, full credit weight.
class LectureCourse : public Course {
public:
    LectureCourse(const std::string& code, const std::string& title, int credits);
    ~LectureCourse() override;

    std::string getCourseType() const override;
    double gradeWeightMultiplier() const override;
};

#endif // LECTURECOURSE_H
