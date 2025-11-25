#include "AuthController.h"
#include <fstream>   // 🌟 lưu dữ liệu khách hàng 
#include <sstream>   // 🌟 lưu dữ liệu khách hàng 
#include <iostream>
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


/// LƯU DỮ LIỆU NGƯỜI DÙNG

void AuthController::loadCustomersFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        // File chưa tồn tại (lần chạy đầu tiên) -> không sao
        return;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string username, password;

        if (!std::getline(ss, username, ';')) continue;
        if (!std::getline(ss, password, ';')) continue;

        // Tránh trùng username với admin hoặc với khách đã có
        bool exists = false;
        for (const auto& u : users) {
            if (u->getUsername() == username) {
                exists = true;
                break;
            }
        }
        if (exists) continue;

        // Tạo customer từ dữ liệu file
        users.push_back(std::make_shared<Customer>(username, password));
    }

    inFile.close();
}

void AuthController::saveCustomersToFile(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cout << "Khong mo duoc file de ghi: " << filename << "\n";
        return;
    }

   for (const auto& u : users) {
        if (u->getRole() == Role::Customer) {
            outFile << u->getUsername() << ";"
                    << u->getPassword()   // 🌟 giờ đã có getter
                    << "\n";
        }
    }

    outFile.close();
}
