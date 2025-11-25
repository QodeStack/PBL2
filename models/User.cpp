#include "User.h"

User::User(const std::string& username, const std::string& password, Role role)
    : username(username), password(password), role(role) {}

std::string User::getUsername() const {
    return username;
}
std::string User::getPassword() const {
    return password;
}

Role User::getRole() const {
    return role;
}

bool User::checkPassword(const std::string& pwd) const {
    return pwd == password;
}

// Admin
Admin::Admin(const std::string& username, const std::string& password)
    : User(username, password, Role::Admin) {}

// Customer
Customer::Customer(const std::string& username, const std::string& password)
    : User(username, password, Role::Customer) {}
