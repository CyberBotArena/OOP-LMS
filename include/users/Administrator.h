#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include "User.h"
#include "Course.h"
#include <vector>
#include <memory>
#include <string>

/*
 * Administrator
 * -------------
 * FR1.2 / FR2.1: creates/updates/removes user accounts and course offerings.
 * The Administrator does not own long-term storage (that is the
 * Repository/Storage layer's job, kept separate per FR5.2) -- it operates
 * on the collections it is given, which keeps this class focused on
 * "administrative rules" rather than file I/O.
 */
class Administrator : public User {
private:
    static int next_admin_num;

    static std::string generateAdminId();
public:
    Administrator(//const std::string& user_Id, 
        const std::string& name, 
        const std::string& username, 
        const std::string& password);
    ~Administrator() override;

    std::string getRole() const override;
    void getMenu() const override;

    // User management (FR1.2)
    //bool removeUser(std::vector<std::unique_ptr<User>>& users, const std::string& username) const;
    //User* findUser(std::vector<std::unique_ptr<User>>& users, const std::string& username) const;

    // Course management (FR2.1)
    //bool removeCourse(std::vector<std::unique_ptr<Course>>& courses, const std::string& courseCode) const;
    //Course* findCourse(std::vector<std::unique_ptr<Course>>& courses, const std::string& courseCode) const;
};

#endif // ADMINISTRATOR_H
