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
#include "./helpers/MenuHelper.h"
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
                while (true)
                {
                    auto [username, password] = view.showLoginForm();

                    if (username == "0")
                        break; // quay lại menu chính

                    std::shared_ptr<User> outUser = nullptr;
                    LoginStatus st = authController.loginDetailed(username, password, outUser);

                    if (st == LoginStatus::Success)
                    {
                        currentUser = outUser;
                        view.showMessageBox("DANG NHAP", {"Dang nhap thanh cong!"}, Color::Green);
                        view.pause();
                        break; // vào menu theo role
                    }
                    else if (st == LoginStatus::WrongPassword)
                    {
                        view.showMessageBox("DANG NHAP", {"Tai Khoan Khong Ton Tai Hoac Sai Mat Khau. Vui Long Thu Lai."},Color::Red);
                        view.pause(); // ✅ phải có, nếu không sẽ bị vẽ lại form ngay => bạn tưởng không hiện
                        continue;     // quay lại form đăng nhập
                    }
                    else // UserNotFound
                    {
                        view.showMessageBox("DANG NHAP", {"Tai Khoan Khong Ton Tai Hoac Sai Mat Khau. Vui Long Thu Lai."},Color::Red);
                        view.pause(); // ✅ phải có
                        continue;
                    }
                }
            }
            else if (choice == 2)
            {
                auto [username, password] = view.showRegisterForm(); // ✅ UI khung

                if (username == "0")
                {
                    continue; // quay lại menu chính
                }

                if (authController.registerCustomer(username, password))
                {
                    authController.saveCustomersToFile(USERS_FILE);
                    view.showMessageBox("DANG KY", {"Dang ky thanh cong! Ban co the dang nhap."},Color::Green);
                }
                else
                {
                    view.showMessageBox("DANG KY", {"Ten nguoi dung da ton tai."},Color::Red);
                }
                view.pause();
            }
            else if (choice == 0)
            {
                TerminalUI ui;
                ui.clear();
                running = false;
                break;
            }
            else
            {
                view.showMessageBox("THONG BAO", {"Lua chon khong hop le."});
                view.pause();
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
                    view.showPitchesScreen(PitchSort::sortedById(pitches));
                    view.pause();
                    break;
                }
                case 2:
                {
                    auto in = view.showCreatePitchForm();
                    std::string msg;
                    bool ok = adminController.createPitch(pitches, in.id, in.name, in.price, in.size, msg);
                    if (ok) view.showMessageBox("KET QUA THEM SAN", {msg},Color::Green);
                    else view.showMessageBox("KET QUA THEM SAN", {msg},Color::Red);
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
                    if (ok) view.showMessageBox("KET QUA CAP NHAT SAN", {msg},Color::Green);
                    else view.showMessageBox("KET QUA CAP NHAT SAN", {msg},Color::Red);
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
                            view.showMessageBox("KET QUA XOA SAN", {"Xoa san thanh cong!"},Color::Green);
                        }
                        else
                        {
                            view.showMessageBox("KET QUA XOA SAN", {"Khong tim thay san."},Color::Red);
                        }
                    }
                    else
                    {
                        view.showMessageBox("KET QUA XOA SAN", {"Da huy thao tac xoa."},Color::Red);
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

                    view.showMessageBox("KET QUA DAT SAN OFFLINE", {msg},Color::Green);
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
                        view.showMessageBox("TINH TIEN SAN", {err},Color::Red);
                        view.pause();
                        break;
                    }

                    view.showMessageBox("CAC BOOKING CHUA TINH TIEN", lines,Color::Cyan);
                    view.pause();

                    int bookingId = view.showCheckoutChooseBookingForm();

                    std::string bill;
                    bool ok = adminController.checkoutPitch(pitches, bookings, pitchId, bookingId, bill);

                    view.showMessageBox("KET QUA TINH TIEN", {bill},Color::Cyan);

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
                    view.pause(); // ✅ THÊM DÒNG NÀY
                    break;
                case 2:
                    customerController.viewFreePitches(pitches, bookings);
                    view.pause(); // ✅ THÊM DÒNG NÀY
                    break;
                case 3:
                    customerController.bookPitch(pitches, bookings, currentUser->getUsername());
                    saveBookingsToFile(bookings, BOOKING_FILE);
                    view.pause();
                    break;
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

    TerminalUI ui;
    ui.clear();
    ui.moveCursor(1, 1); // ✅ tránh in lệch vị trí
    std::cout << "Tam bietttttt!\n";
    return 0;
}
