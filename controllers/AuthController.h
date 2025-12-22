#ifndef AUTH_CONTROLLER_H
#define AUTH_CONTROLLER_H

#include <vector>
#include <memory>
#include <string>
#include "../models/User.h"


enum class LoginStatus {
    Success,
    UserNotFound,
    WrongPassword
};
class AuthController {
private:
    std::vector<std::shared_ptr<User>> users;

public:
    AuthController();

    LoginStatus loginDetailed(const std::string& username,
                          const std::string& password,
                          std::shared_ptr<User>& outUser);

    std::shared_ptr<User> login(const std::string& username, const std::string& password);
    bool registerCustomer(const std::string& username, const std::string& password);

      // 🌟 THÊM 2 HÀM NÀY : lưu dữ liệu khách hàng 
    void loadCustomersFromFile(const std::string& filename);
    void saveCustomersToFile(const std::string& filename) const;
};

#endif // AUTH_CONTROLLER_H

