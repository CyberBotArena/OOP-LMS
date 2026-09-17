#ifndef LECTURER_H
#define LECTURER_H

#include "User.h"

#include <string>
#include <vector>

class Lecturer : public User {
private:
    std::vector<std::string> assignedCourseCodes;

public:
    Lecturer(
        const std::string& name,
        const std::string& username,
        const std::string& password
    );

    ~Lecturer() override;

    std::string getRole() const override;

    void showDashboard() const override;

    const std::vector<std::string>&
    getAssignedCourses() const;

    bool teaches(
        const std::string& courseCode
    ) const;

    void assignCourse(
        const std::string& courseCode
    );

    void unassignCourse(
        const std::string& courseCode
    );
};

#endif