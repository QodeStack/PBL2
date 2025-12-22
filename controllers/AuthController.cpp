#include "AuthController.h"
#include <iostream>
#include "../storage/DataStorage.h"
AuthController::AuthController() {
    // Tạo sẵn 1 admin mặc định
    users.push_back(std::make_shared<Admin>("admin", "123"));
}

LoginStatus AuthController::loginDetailed(const std::string& username,
                                          const std::string& password,
                                          std::shared_ptr<User>& outUser)
{
    // 1) Duyệt danh sách users (đã load từ file)
    for (const auto& u : users)
    {
        if (!u) continue;

        if (u->getUsername() == username)
        {
            if (u->getPassword() == password)
            {
                outUser = u; // ✅ trả thẳng shared_ptr<User> đang có
                return LoginStatus::Success;
            }
            return LoginStatus::WrongPassword;
        }
    }

    return LoginStatus::UserNotFound;
}



std::shared_ptr<User> AuthController::login(const std::string& username,
                                            const std::string& password)
{
    std::shared_ptr<User> out = nullptr;
    LoginStatus st = loginDetailed(username, password, out);
    if (st == LoginStatus::Success) return out;
    return nullptr;
}


bool AuthController::registerCustomer(const std::string& username, const std::string& password) {
    // Kiểm tra trùng username
    for (auto& u : users) {
        if (u->getUsername() == username) {
            return false;
        }
    }
    users.push_back(std::make_shared<Customer>(username, password));
    return true;
}



void AuthController::loadCustomersFromFile(const std::string& filename)
{
    loadUsersFromFile(users, filename);
}
void AuthController::saveCustomersToFile(const std::string& filename) const
{
    saveUsersToFile(users, filename);
}