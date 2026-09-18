#include "User.h"

#include <stdexcept>

using namespace std;


// Constructor
User::User(
    //const std::string& user_Id,
    const std::string& name,
    const std::string& user_name,
    const std::string& password
)
    : user_Id(user_Id),
      name(name),
      user_name(user_name),
      password(password)
{

    if (name.empty())
    {
        throw invalid_argument(
            "Name cannot be empty."
        );
    }

    if (user_name.empty())
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
string User::getUserId() const
{
    return user_Id;
}

// Get name
const string& User::getName() const
{
    return name;
}


// Get username
const string& User::getUsername() const
{
    return user_name;
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
/*ostream& operator<<(
    ostream& os,
    const User& user)
{
    os << "[" << user.getRole() << "] "
       << "ID: " << user.user_Id
       << " | Username: " << user.user_name
       << " | Name: " << user.name;

    return os;
}*/