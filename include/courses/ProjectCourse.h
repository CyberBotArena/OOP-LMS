#ifndef PROJECTCOURSE_H
#define PROJECTCOURSE_H

#include "Course.h"

// Project-based course: group work, graded mostly on the final deliverable.
class ProjectCourse : public Course {
private:
    int groupSize;

public:
    ProjectCourse(const std::string& code, const std::string& title, int credits, int capacity, int groupSize);
    ~ProjectCourse() override;

    int getGroupSize() const;

    std::string getCourseType() const override;
    double gradeWeightMultiplier() const override;
};

#endif // PROJECTCOURSE_H
