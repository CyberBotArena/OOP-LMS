#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>

class User {
private:
    static int nextId;

protected:
    int userId;
    std::string name;
    std::string username;
    std::string password;

public:
    User(
        const std::string& name,
        const std::string& username,
        const std::string& password
    );

    virtual ~User();

    int getUserId() const;
    const std::string& getName() const;
    const std::string& getUsername() const;

    void setName(const std::string& newName);

    bool checkPassword(
        const std::string& attempt
    ) const;

    virtual std::string getRole() const = 0;

    virtual void showDashboard() const = 0;

    friend std::ostream& operator<<(
        std::ostream& os,
        const User& user
    );
};

#endif