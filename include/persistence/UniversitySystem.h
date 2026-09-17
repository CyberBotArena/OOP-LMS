#ifndef UNIVERSITY_SYSTEM_H
#define UNIVERSITY_SYSTEM_H

#include <string>
#include <vector>

#include "../courses/Course.h"
#include "../courses/CourseOffering.h"
#include "../persistence/Repository.h"
#include "../persistence/Storage.h"
#include "../users/User.h"

class UniversitySystem {
private:
    Repository<User> users;
    Repository<Course> courses;
    Repository<CourseOffering> offerings;

    Storage* storage;
    User* current_user;

public:
    UniversitySystem(Storage* storage_pointer);

    void addUser(User* user);
    User* findUser(const std::string& user_id) const;
    void removeUser(const std::string& user_id);
    std::vector<User*> getAllUsers() const;

    void addCourse(Course* course);
    Course* findCourse(const std::string& course_code) const;

    void addOffering(CourseOffering* offering);
    CourseOffering* findOffering(
        const std::string& offering_id
    ) const;

    void setCurrentUser(User* user);
    User* getCurrentUser() const;

    void save();
    void load();
};

#endif