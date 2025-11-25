#include "AuthController.h"

AuthController::AuthController() {
    // Tạo sẵn 1 admin mặc định
    users.push_back(std::make_shared<Admin>("admin", "123"));
}

std::shared_ptr<User> AuthController::login(const std::string& username, const std::string& password) {
    for (auto& u : users) {
        if (u->getUsername() == username && u->checkPassword(password)) {
            return u;
        }
    }
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
