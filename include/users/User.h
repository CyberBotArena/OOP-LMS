#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>

class User {

protected:
    std::string user_Id;
    std::string name;
    std::string user_name;
    std::string password;

public:
    User(
        //const std::string& user_ID,
        const std::string& name,
        const std::string& user_name,
        const std::string& password
    );

    virtual ~User();

    std::string getUserId() const;
    const std::string& getName() const;
    const std::string& getUsername() const;

    void setName(const std::string& new_name);

    bool checkPassword(
        const std::string& psswrd
    ) const;

    virtual std::string getRole() const = 0;

    virtual void getMenu() const = 0;

    /*friend std::ostream& operator<<(
        std::ostream& os,
        const User& user
    );*/
};

#endif