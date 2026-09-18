#include "Administrator.h"
#include <algorithm>
#include <iostream>

using namespace std;

int Administrator::next_admin_num = 1001;


Administrator::Administrator(

    //const string& user_Id, 
    const string& name, 
    const string& username, 
    const string& password)
    : User(name, username, password) {}

Administrator::~Administrator() {}

// generate student id
string Administrator::generateAdminId()
{
    return "ADM" + std::to_string(next_admin_num++);
}

string Administrator::getRole() const {
    return "Administrator";
}

void Administrator::getMenu() const {
    cout << "--- Administrator Dashboard: " << getName() << " ---\n"
              << "1. Create user account\n"
              << "2. Remove user account\n"
              << "3. Create course offering\n"
              << "4. Edit/remove course offering\n"
              << "5. Generate reports\n"
              << "6. Logout\n";
}

// Note on "create": creating a NEW Student/Lecturer/Administrator or a new
// LectureCourse/LabCourse/ProjectCourse means picking a concrete derived
// type, so that step naturally happens at the menu/UI layer (which knows
// what the admin chose) via e.g. std::make_unique<Student>(...); this
// object is then handed to the storage layer to persist (FR5.2).
// The methods below cover the "update/remove" side, which IS this class's
// job: enforcing administrative rules over an existing collection.

/*User* Administrator::findUser(vector<unique_ptr<User>>& users, const string& username) const {
    for (auto& u : users) {
        if (u->getUsername() == username) return u.get();
    }
    return nullptr;
}

bool Administrator::removeUser(vector<unique_ptr<User>>& users, const string& username) const {
    auto it = remove_if(users.begin(), users.end(),
        [&](const unique_ptr<User>& u) { return u->getUsername() == username; });
    if (it == users.end()) return false;
    users.erase(it, users.end());
    return true;
}

Course* Administrator::findCourse(vector<unique_ptr<Course>>& courses, const string& courseCode) const {
    for (auto& c : courses) {
        if (c->getCode() == courseCode) return c.get();
    }
    return nullptr;
}

bool Administrator::removeCourse(vector<unique_ptr<Course>>& courses, const string& courseCode) const {
    auto it = remove_if(courses.begin(), courses.end(),
        [&](const unique_ptr<Course>& c) { return c->getCode() == courseCode; });
    if (it == courses.end()) return false;
    courses.erase(it, courses.end());
    return true;
}*/
