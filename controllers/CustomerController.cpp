#include "CustomerController.h"
#include <iostream>
//// các thư viện để setup thời gian 
#include <limits> 
/////

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
                  << " | Loai san: " << p.getSize() << " nguoi"
                  << " | Trang thai: " << (p.getIsBooked() ? "Da dat" : "Trong");
        if (p.getIsBooked()) {
            std::cout << " | Khach: " << p.getBookedBy();
        }
        std::cout << '\n';
    }
}

void CustomerController::viewFreePitches(const std::vector<Pitch>& pitches) const {
    std::cout << "\n=== SAN TRONG ===\n";
    bool found = false;
    for (const auto& p : pitches) {
        if (!p.getIsBooked()) {
            std::cout << "ID: " << p.getId() << " | Ten: " << p.getName() << '\n';
            found = true;
        }
    }
    if (!found) std::cout << "Khong co san trong.\n";
}

void CustomerController::viewBookedPitches(const std::vector<Pitch>& pitches) const {
    std::cout << "\n=== SAN DA DAT ===\n";
    bool found = false;
    for (const auto& p : pitches) {
        if (p.getIsBooked()) {
            std::cout << "ID: " << p.getId()
                      << " | Ten: " << p.getName()
                      << " | Gia: " << p.getPrice()
                      << " | Loai san: " << p.getSize() << " nguoi"
                      << " | Khach: " << p.getBookedBy()
                      << " | Thoi gian dat: " << p.getBookedTime()
                      << '\n';
            found = true;
        }
    }
    if (!found) std::cout << "Chua co san nao duoc dat.\n";
}

void CustomerController::bookPitch(std::vector<Pitch>& pitches, const std::string& username) {
    int id;
    std::cout << "Nhap ID san muon dat: ";
    std::cin >> id;

    for (auto& p : pitches) {

        if (p.getId() == id) {
            if (p.getIsBooked()) {
                std::cout << "San nay da duoc dat.\n";
            } else {
                    // Xóa \n còn dư sau khi nhập id
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::string desiredTime;
                std::cout << "Nhap thoi gian muon dat (VD: 2025-11-30 18:00): ";
                std::getline(std::cin, desiredTime);

                p.setBooked(true);
                p.setBookedBy(username);
                p.setBookedTime(desiredTime);   // 💡 lưu lại thời gian khách muốn đặt

                std::cout << "Dat san thanh cong vao khung gio: " << p.getBookedTime() << "\n";
            return;
        }
    }
    std::cout << "Khong tim thay san.\n";
}
}
