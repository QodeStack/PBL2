#ifndef USER_H
#define USER_H

#include <string> 

// Danh Sách phân quyền: trong dự án này cần phân quyền 2 cái chính là admin(người quản lý) và customer(người sử dụng)
enum class Role {
    Admin,
    Customer
};

class User { // Lớp cha : cơ sở cho các lớp admin và customer 
protected:
    std::string username;
    std::string password;
    Role role;
public:
    User(const std::string& username, const std::string& password, Role role); // Hàm khởi tại (constructor) : tạo 1 user với username,password và role 
    virtual ~User() = default; // Hàm Hủy (Destructor)

    
    std::string getUsername() const; // hàm lấy tên người dùng trong object đang gọi 
    std::string getPassword() const; // hàm lấy mật khẩu người dùng trong object đang gọi 
    Role getRole() const; // hàm lấy quyền của người dùng trong object đang gọi để phân menu  
    bool checkPassword(const std::string& pwd) const; // hàm kiểm tra người dùng có nhập đúng không mật khẩu không để cho phép truy cập 
};

class Admin : public User { // Lớp con kế thừa từ user 
public:
    Admin(const std::string& username, const std::string& password);
};

class Customer : public User { // lớp con kế thừ từ customer
public:
    Customer(const std::string& username, const std::string& password);
};

#endif // USER_H
