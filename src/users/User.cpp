#include "User.h"

#include <stdexcept>

using namespace std;


int User::nextId = 1000;


// Constructor
User::User(
    const string& name,
    const string& username,
    const string& password
)
    : userId(nextId++),
      name(name),
      username(username),
      password(password)
{
    if (name.empty())
    {
        throw invalid_argument(
            "Name cannot be empty."
        );
    }

    if (username.empty())
    {
        throw invalid_argument(
            "Username cannot be empty."
        );
    }

    if (password.empty())
    {
        throw invalid_argument(
            "Password cannot be empty."
        );
    }
}


// Virtual destructor
User::~User()
{
}


// Get user ID
int User::getUserId() const
{
    return userId;
}


// Get name
const string& User::getName() const
{
    return name;
}


// Get username
const string& User::getUsername() const
{
    return username;
}


// Change name
void User::setName(
    const string& newName)
{
    if (newName.empty())
    {
        throw invalid_argument(
            "Name cannot be empty."
        );
    }

    name = newName;
}


// Check login password
bool User::checkPassword(
    const string& attempt) const
{
    return attempt == password;
}


// Output operator
ostream& operator<<(
    ostream& os,
    const User& user)
{
    os << "[" << user.getRole() << "] "
       << "ID: " << user.userId
       << " | Username: " << user.username
       << " | Name: " << user.name;

    return os;
}