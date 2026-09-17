#include "UniversitySystem.h"

#include <stdexcept>

using namespace std;


// Constructor
UniversitySystem::UniversitySystem(
    Storage* storage_pointer)
    : storage(storage_pointer),
      current_user(nullptr)
{
}


// Add user
void UniversitySystem::addUser(User* user)
{
    if (user == nullptr)
    {
        throw invalid_argument(
            "User cannot be null."
        );
    }

    users.add(user->getId(), user);
}


// Find user
User* UniversitySystem::findUser(
    const string& user_id) const
{
    return users.find(user_id);
}


// Remove user
void UniversitySystem::removeUser(
    const string& user_id)
{
    users.remove(user_id);
}


// Get all users
vector<User*> UniversitySystem::getAllUsers() const
{
    return users.getAll();
}


// Add course
void UniversitySystem::addCourse(Course* course)
{
    if (course == nullptr)
    {
        throw invalid_argument(
            "Course cannot be null."
        );
    }

    courses.add(course->getCode(), course);
}


// Find course
Course* UniversitySystem::findCourse(
    const string& course_code) const
{
    return courses.find(course_code);
}


// Add course offering
void UniversitySystem::addOffering(
    CourseOffering* offering)
{
    if (offering == nullptr)
    {
        throw invalid_argument(
            "Course offering cannot be null."
        );
    }

    offerings.add(
        offering->getOfferingId(),
        offering
    );
}


// Find course offering
CourseOffering* UniversitySystem::findOffering(
    const string& offering_id) const
{
    return offerings.find(offering_id);
}


// Set currently logged-in user
void UniversitySystem::setCurrentUser(User* user)
{
    current_user = user;
}


// Get currently logged-in user
User* UniversitySystem::getCurrentUser() const
{
    return current_user;
}


// Save system data
void UniversitySystem::save()
{
    if (storage == nullptr)
    {
        throw runtime_error(
            "Storage has not been configured."
        );
    }

    storage->saveAll();
}


// Load system data
void UniversitySystem::load()
{
    if (storage == nullptr)
    {
        throw runtime_error(
            "Storage has not been configured."
        );
    }

    storage->loadAll();
}