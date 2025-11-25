#include <iostream>
#include <vector>
#include <memory>

#include "models/Pitch.h"
#include "models/User.h"
#include "controllers/AuthController.h"
#include "controllers/AdminController.h"
#include "controllers/CustomerController.h"
#include "views/MenuView.h"

int main() {
    std::vector<Pitch> pitches;
    AuthController authController;
    AdminController adminController;
    CustomerController customerController;
    MenuView view;

    std::shared_ptr<User> currentUser = nullptr;
    bool running = true;

    while (running) {
        if (!currentUser) {
            int choice = view.showMainMenu();
            if (choice == 1) {
                std::string username, password;
                std::cout << "Username: ";
                std::cin >> username;
                std::cout << "Password: ";
                std::cin >> password;

                auto user = authController.login(username, password);
                if (user) {
                    currentUser = user;
                    std::cout << "Dang nhap thanh cong! Xin chao, " << username << "\n";
                } else {
                    std::cout << "Sai thong tin dang nhap.\n";
                }
            } else if (choice == 2) {
                std::string username, password;
                std::cout << "Nhap username: ";
                std::cin >> username;
                std::cout << "Nhap password: ";
                std::cin >> password;

                if (authController.registerCustomer(username, password)) {
                    std::cout << "Dang ky thanh cong! Ban co the dang nhap.\n";
                } else {
                    std::cout << "Username da ton tai.\n";
                }
            } else if (choice == 0) {
                running = false;
            } else {
                std::cout << "Lua chon khong hop le.\n";
            }
        } else {
            // Đã đăng nhập
            if (currentUser->getRole() == Role::Admin) {
                int choice = view.showAdminMenu();
                switch (choice) {
                    case 1:
                        adminController.listPitches(pitches);
                        break;
                    case 2:
                        adminController.createPitch(pitches);
                        break;
                    case 3:
                        adminController.updatePitch(pitches);
                        break;
                    case 4:
                        adminController.deletePitch(pitches);
                        break;
                    case 0:
                        std::cout << "Dang xuat...\n";
                        currentUser.reset();
                        break;
                    default:
                        std::cout << "Lua chon khong hop le.\n";
                }
            } else { // Customer
                int choice = view.showCustomerMenu();
                switch (choice) {
                    case 1:
                        customerController.viewAllPitches(pitches);
                        break;
                    case 2:
                        customerController.viewFreePitches(pitches);
                        break;
                    case 3:
                        customerController.viewBookedPitches(pitches);
                        break;
                    case 4:
                        customerController.bookPitch(
                            pitches,
                            currentUser->getUsername()
                        );
                        break;
                    case 0:
                        std::cout << "Dang xuat...\n";
                        currentUser.reset();
                        break;
                    default:
                        std::cout << "Lua chon khong hop le.\n";
                }
            }
        }
    }

    std::cout << "Tam biet!\n";
    return 0;
}
