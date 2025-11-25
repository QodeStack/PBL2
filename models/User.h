#ifndef USER_H
#define USER_H

#include <string>

enum class Role {
    Admin,
    Customer
};

class User {
protected:
    std::string username;
    std::string password;
    Role role;

public:
    User(const std::string& username, const std::string& password, Role role);
    virtual ~User() = default;

    std::string getUsername() const;
    std::string getPassword() const; // THÊM ĐỂ LƯU MẬT KHẨU VÀO FILE TEXT
    Role getRole() const;
    bool checkPassword(const std::string& pwd) const;
};

class Admin : public User {
public:
    Admin(const std::string& username, const std::string& password);
};

class Customer : public User {
public:
    Customer(const std::string& username, const std::string& password);
};

#endif // USER_H
