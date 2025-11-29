#include "CustomerController.h"
#include <iostream>
#include <limits>  // để dùng std::numeric_limits


void CustomerController::viewAllPitches(const std::vector<Pitch>& pitches) const {
    std::cout << "\n=== TAT CA SAN ===\n";
    if (pitches.empty()) {
        std::cout << "Chua co san nao.\n";
        return;
    }
    for (const auto& p : pitches) {
        std::cout << "ID: " << p.getId()
                  << " | Ten: " << p.getName()
                  << " | Gia: " << p.getPrice()
                  << " | Loai san: " << p.getSize() << " nguoi\n";
    }
}

void CustomerController::viewFreePitches(const std::vector<Pitch>& pitches,
                                         const std::vector<Booking>& bookings) const {
    // Xóa \n còn lại trong buffer trước khi dùng getline
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string timeSlot;
    std::cout << "Nhap khung gio muon kiem tra (VD: 2025-12-01 18:00-19:00): ";
    std::getline(std::cin, timeSlot);

    std::cout << "\n=== SAN TRONG O KHUNG GIO " << timeSlot << " ===\n";
    bool found = false;

    for (const auto& p : pitches) {
        bool occupied = false;

        // Kiểm tra xem sân này có booking nào trùng khung giờ không
        for (const auto& b : bookings) {
            if (b.getPitchId() == p.getId() && b.getTimeSlot() == timeSlot) {
                occupied = true;
                break;
            }
        }

        if (!occupied) {
            std::cout << "ID: " << p.getId()
                      << " | Ten: " << p.getName()
                      << " | Gia: " << p.getPrice()
                      << " | Loai san: " << p.getSize() << " nguoi\n";
            found = true;
        }
    }

    if (!found) {
        std::cout << "Khong co san trong o khung gio nay.\n";
    }
}

void CustomerController::viewBookedPitches(const std::vector<Pitch>& pitches,
                                           const std::vector<Booking>& bookings) const {
    std::cout << "\n=== CAC LICH DAT SAN ===\n";

    if (bookings.empty()) {
        std::cout << "Chua co lich dat nao.\n";
        return;
    }

    for (const auto& b : bookings) {
        // Tìm thông tin sân tương ứng
        const Pitch* pitchPtr = nullptr;
        for (const auto& p : pitches) {
            if (p.getId() == b.getPitchId()) {
                pitchPtr = &p;
                break;
            }
        }

        std::cout << "Booking ID: " << b.getId()
                  << " | San ID: " << b.getPitchId();

        if (pitchPtr) {
            std::cout << " (" << pitchPtr->getName()
                      << ", " << pitchPtr->getSize() << " nguoi"
                      << ", Gia: " << pitchPtr->getPrice() << ")";
        }

        std::cout << " | Khach: " << b.getCustomerUsername()
                  << " | Khung gio: " << b.getTimeSlot()
                  << '\n';
    }
}

void CustomerController::bookPitch(const std::vector<Pitch>& pitches,
                                   std::vector<Booking>& bookings,
                                   const std::string& username) {
    int id;
    std::cout << "Nhap ID san muon dat: ";
    std::cin >> id;

    // Tìm sân theo ID
    const Pitch* selectedPitch = nullptr;
    for (const auto& p : pitches) {
        if (p.getId() == id) {
            selectedPitch = &p;
            break;
        }
    }

    if (!selectedPitch) {
        std::cout << "Khong tim thay san.\n";
        return;
    }

    // Xóa \n còn lại trong buffer
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string timeSlot;
    std::cout << "Nhap khung gio muon dat (VD: 2025-12-01 18:00-19:00): ";
    std::getline(std::cin, timeSlot);

    // Kiểm tra xem sân này ở khung giờ này đã có booking chưa
    for (const auto& b : bookings) {
        if (b.getPitchId() == id && b.getTimeSlot() == timeSlot) {
            std::cout << "Khung gio nay cho san nay da duoc dat. Vui long chon khung gio khac.\n";
            return;
        }
    }

    // Nếu chưa ai đặt -> tạo booking mới
    bookings.emplace_back(
    id,           // ID sân
    username,          // tài khoản khách
    timeSlot,          // ví dụ: "2025-12-01 18:00-20:00"
    "",                // startTime (online mình không dùng giờ máy, mà tính từ timeSlot)
    "",                // endTime (chưa cần)
    BookingStatus::Active,
    0.0
);

    std::cout << "Dat san thanh cong! San " << selectedPitch->getName()
              << " vao khung gio: " << timeSlot << "\n";
}