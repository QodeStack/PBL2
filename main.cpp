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

#include "views/TerminalUI.h"

int main()
{
    std::vector<Pitch> pitches;
    std::vector<Booking> bookings;
    AuthController authController;
    AdminController adminController;
    CustomerController customerController;
    MenuView view;

    // 🌟 Load dữ liệu từ file (nếu có)
    const std::string PITCH_FILE = "data/pitches.txt";
    const std::string BOOKING_FILE = "data/bookings.txt";
    const std::string USERS_FILE = "data/users.txt";

    loadPitchesFromFile(pitches, PITCH_FILE);
    loadBookingsFromFile(bookings, BOOKING_FILE);
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
                auto [username, password] = view.showLoginForm(); // ✅ UI đẹp trong khung

                if (username == "0")
                {
                    continue; // quay lại menu chính
                }

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
                {
                    view.showPitchesScreen(pitches); // ✅ UI ra giữa
                    view.pause();
                    break;
                }
                case 2:
                {
                    auto in = view.showCreatePitchForm();

                    std::string msg;
                    bool ok = adminController.createPitch(pitches, in.id, in.name, in.price, in.size, msg);
                    view.showMessageBox("KET QUA THEM SAN", {msg});
                    if (ok)
                    {
                        savePitchesToFile(pitches, PITCH_FILE);
                    }
                    view.pause();
                    break;
                }
                case 3:
                {
                    auto in = view.showUpdatePitchForm();
                    std::string msg;
                    bool ok = adminController.updatePitch(pitches, in.id, in.newName, in.newPrice, in.newSize, msg);
                    view.showMessageBox("KET QUA CAP NHAT SAN", {msg});
                    if (ok)
                        savePitchesToFile(pitches, PITCH_FILE);
                    view.pause();
                    break;
                }
                case 4:
                {
                    auto [id, confirm] = view.showDeletePitchForm();
                    if (confirm)
                    {
                        bool ok = adminController.deletePitchById(pitches, id);
                        if (ok)
                        {
                            savePitchesToFile(pitches, PITCH_FILE);
                            view.showMessageBox("KET QUA XOA SAN", {"Xoa san thanh cong!"});
                        }
                        else
                        {
                            view.showMessageBox("KET QUA XOA SAN", {"Khong tim thay san."});
                        }
                    }
                    else
                    {
                        view.showMessageBox("KET QUA XOA SAN", {"Da huy thao tac xoa."});
                    }
                    view.pause();
                    break;
                }
                case 5:
                {
                    auto in = view.showOfflineBookingForm(); // (pitchId, customerName)

                    std::string msg;
                    bool ok = adminController.bookPitchOffline(pitches, bookings, in.pitchId, in.customerName, msg);

                    if (ok)
                        saveBookingsToFile(bookings, BOOKING_FILE);

                    view.showMessageBox("KET QUA DAT SAN OFFLINE", {msg});
                    view.pause();
                    break;
                }
                case 6:
                {
                    view.showUnpaidBookingsScreen(bookings, pitches);
                    view.pause();
                    break;
                }
                case 7:
                {
                    int pitchId = view.showCheckoutChoosePitchForm();

                    std::vector<std::string> lines;
                    std::string err;
                    if (!adminController.getCheckoutCandidates(pitches, bookings, pitchId, lines, err))
                    {
                        view.showMessageBox("TINH TIEN SAN", {err});
                        view.pause();
                        break;
                    }

                    view.showMessageBox("CAC BOOKING CHUA TINH TIEN", lines);
                    view.pause();

                    int bookingId = view.showCheckoutChooseBookingForm();

                    std::string bill;
                    bool ok = adminController.checkoutPitch(pitches, bookings, pitchId, bookingId, bill);

                    view.showMessageBox("KET QUA TINH TIEN", {bill});

                    if (ok)
                    {
                        saveBookingsToFile(bookings, BOOKING_FILE);
                        savePitchesToFile(pitches, PITCH_FILE);
                    }

                    view.pause();
                    break;
                }

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
