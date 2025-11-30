#include <iostream>
#include <vector>
#include <memory>

#include "models/Pitch.h"
#include "models/User.h"
#include "models/Booking.h"
#include "controllers/AuthController.h"
#include "controllers/AdminController.h"
#include "controllers/CustomerController.h"
#include "views/MenuView.h"
#include "storage/DataStorage.h"  

int main()
{
    std::vector<Pitch> pitches;
    std::vector<Booking> bookings;
    AuthController authController;
    AdminController adminController;
    CustomerController customerController;
    MenuView view;

    // 🌟 Load dữ liệu từ file (nếu có)
    const std::string PITCH_FILE   = "data/pitches.txt";
    const std::string BOOKING_FILE = "data/bookings.txt";
    const std::string USERS_FILE = "data/users.txt";

    loadPitchesFromFile(pitches,PITCH_FILE );
    loadBookingsFromFile(bookings,BOOKING_FILE);
    authController.loadCustomersFromFile(USERS_FILE);


    std::shared_ptr<User> currentUser = nullptr;
    bool running = true;

    while (running)
    {
        if (!currentUser)
        {
            int choice = view.showMainMenu();
            if (choice == 1)
            {
                std::string username, password;
                std::cout << "Username: ";
                std::cin >> username;
                std::cout << "Password: ";
                std::cin >> password;

                auto user = authController.login(username, password);
                if (user)
                {
                    currentUser = user;
                    std::cout << "Dang nhap thanh cong! Xin chao, " << username << "\n";
                }
                else
                {
                    std::cout << "Sai thong tin dang nhap.\n";
                }
            }
            else if (choice == 2)
            {
                std::string username, password;
                std::cout << "Nhap username: ";
                std::cin >> username;
                std::cout << "Nhap password: ";
                std::cin >> password;

                if (authController.registerCustomer(username, password))
                {
                    std::cout << "Dang ky thanh cong! Ban co the dang nhap.\n";
                }
                else
                {
                    std::cout << "Username da ton tai.\n";
                }
            }
            else if (choice == 0)
            {
                running = false;
            }
            else
            {
                std::cout << "Lua chon khong hop le.\n";
            }
        }
        else
        {
            // Đã đăng nhập
            if (currentUser->getRole() == Role::Admin)
            {
                int choice = view.showAdminMenu();
                switch (choice)
                {
                case 1:
                    adminController.listPitches(pitches);
                    break;
                case 2:
                    adminController.createPitch(pitches);
                    savePitchesToFile(pitches, PITCH_FILE); 
                    break;
                case 3:
                    adminController.updatePitch(pitches);
                    savePitchesToFile(pitches, PITCH_FILE);
                    break;
                case 4:
                    adminController.deletePitch(pitches);
                     savePitchesToFile(pitches, PITCH_FILE);
                    break;
                case 5:
                    adminController.bookPitchOffline(pitches, bookings);  
                    saveBookingsToFile(bookings, BOOKING_FILE); 
                    break;
                case 6:  // ✅ Xem các lịch CHƯA tính tiền
                    adminController.viewUnpaidBookings(bookings, pitches);
                    break;
                case 7:
                    adminController.checkoutPitch(pitches, bookings); 
                    saveBookingsToFile(bookings, BOOKING_FILE);
                    savePitchesToFile(pitches, PITCH_FILE);
                    break;
                case 0:
                    std::cout << "Dang xuat...\n";
                    currentUser.reset();
                    break;
                default:
                    std::cout << "Lua chon khong hop le.\n";
                }
            }
            else
            { // Customer
                int choice = view.showCustomerMenu();
                switch (choice)
                {
                case 1:
                    customerController.viewAllPitches(pitches);
                    break;
                case 2:
                    customerController.viewFreePitches(pitches, bookings);
                    break;
                case 3:
                    customerController.bookPitch(
                        pitches,
                        bookings,
                        currentUser->getUsername());
                        saveBookingsToFile(bookings, BOOKING_FILE);
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

    // 🌟 Trước khi thoát, lưu lại dữ liệu user
    authController.saveCustomersToFile(USERS_FILE); 
    
    std::cout << "Tam biet!\n";
    return 0;
}
