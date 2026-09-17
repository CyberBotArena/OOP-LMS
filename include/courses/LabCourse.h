#ifndef LABCOURSE_H
#define LABCOURSE_H

#include "Course.h"

// Lab-based course: has extra weekly lab hours and a heavier practical weighting.
class LabCourse : public Course {
private:
    int labHoursPerWeek;

public:
    LabCourse(const std::string& code, const std::string& title, int credits, int capacity, int labHoursPerWeek);
    ~LabCourse() override;

    int getLabHoursPerWeek() const;

    std::string getCourseType() const override;
    double gradeWeightMultiplier() const override;
};

#endif // LABCOURSE_H
