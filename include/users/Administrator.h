#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include "User.h"
#include "Course.h"

#include <memory>
#include <string>
#include <vector>

class Administrator : public User {
public:
    Administrator(
        const std::string& name,
        const std::string& username,
        const std::string& password
    );

    ~Administrator() override;

    std::string getRole() const override;

    void showDashboard() const override;


    // User management

    bool removeUser(
        std::vector<std::unique_ptr<User>>& users,
        const std::string& username
    ) const;

    User* findUser(
        std::vector<std::unique_ptr<User>>& users,
        const std::string& username
    ) const;


    // Course management

    bool removeCourse(
        std::vector<std::unique_ptr<Course>>& courses,
        const std::string& courseCode
    ) const;

    Course* findCourse(
        std::vector<std::unique_ptr<Course>>& courses,
        const std::string& courseCode
    ) const;
};

#endif