#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <vector>
#include <iostream>

/*
 * Course
 * ------
 * Abstract base for the three course types (FR2.2): LectureCourse,
 * LabCourse, ProjectCourse. Differing grading/credit behaviour is
 * expressed through the pure virtual gradeWeightMultiplier() and
 * getCourseType(), used polymorphically wherever courses are reported on.
 *
 * Composition: a Course OWNS its list of prerequisite codes and its own
 * capacity bookkeeping -- that data has no meaning outside this Course and
 * is destroyed with it.
 * Aggregation: a Course refers to its Lecturer by username (identity),
 * it does NOT own the Lecturer object -- that object's lifetime is
 * managed by the user repository elsewhere.
 */
class Course {
protected:
    std::string code;
    std::string title;
    int credits;
    int capacity;
    int enrolledCount;
    std::string lecturerUsername;           // aggregation: reference by identity only
    std::vector<std::string> prerequisites; // composition: owned by this Course

public:
    Course(const std::string& code, const std::string& title, int credits, int capacity);

    virtual ~Course();

    // --- const-correct getters ---
    const std::string& getCode() const;
    const std::string& getTitle() const;
    int getCredits() const;
    int getCapacity() const;
    int getEnrolledCount() const;
    const std::string& getLecturerUsername() const;
    const std::vector<std::string>& getPrerequisites() const;

    // --- behaviour ---
    bool hasCapacity() const;
    void incrementEnrolled();
    void decrementEnrolled();
    void setLecturer(const std::string& lecturerUsername);
    void addPrerequisite(const std::string& courseCode);
    bool requiresPrerequisite(const std::string& courseCode) const;

    // Pure virtual -> Course is abstract; each subtype defines its own
    // identity and grading behaviour (FR2.2).
    virtual std::string getCourseType() const = 0;
    virtual double gradeWeightMultiplier() const = 0;

    friend std::ostream& operator<<(std::ostream& os, const Course& course);
};

#endif // COURSE_H
