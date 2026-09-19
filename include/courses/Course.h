#ifndef COURSE_H
#define COURSE_H

#include <iostream>
#include <string>
#include <vector>

class Course {
protected:
    std::string code;
    std::string title;
    int credits;
    std::vector<std::string> prerequisites;

public:
    Course(const std::string& code,
           const std::string& title,
           int credits);

    virtual ~Course();

    const std::string& getCode() const;
    const std::string& getTitle() const;
    int getCredits() const;
    const std::vector<std::string>& getPrerequisites() const;

    void addPrerequisite(const std::string& courseCode);
    bool requiresPrerequisite(const std::string& courseCode) const;

    // Polymorphic behaviour required by the Course hierarchy.
    virtual std::string getCourseType() const = 0;
    virtual double gradeWeightMultiplier() const = 0;

    friend std::ostream& operator<<(std::ostream& os, const Course& course);
};

#endif
