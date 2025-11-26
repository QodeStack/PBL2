#include "AdminController.h"
#include <iostream>
#include <limits>   // <-- THÊM DÒNG NÀY

void AdminController::listPitches(const std::vector<Pitch>& pitches) const {
    std::cout << "\n=== DANH SACH SAN ===\n";
    if (pitches.empty()) {
        std::cout << "Chua co san nao.\n";
        return;
    }
    for (const auto& p : pitches) {
        std::cout << "ID: " << p.getId()
                  << " | Ten: " << p.getName()
                  << " | Gia: " << p.getPrice()
                  << " | Loai san: " << p.getSize() << " nguoi";
        std::cout << '\n';
    }
}

void AdminController::createPitch(std::vector<Pitch>& pitches) {
    int id;
    std::string name;
    double price;
    int size;

    std::cout << "Nhap ID san: ";
    std::cin >> id;
    std::cin.ignore();
    std::cout << "Nhap ten san: ";
    std::getline(std::cin, name);
    std::cout << "Nhap gia san (VD: 200000): ";
    std::cin >> price;
    std::cout << "Nhap loai san (5, 7, 11): ";
    std::cin >> size;

    if (size != 5 && size != 7 && size != 11) {
        std::cout << "Loai san khong hop le! Chi chap nhan 5, 7, 11.\n";
        return;
    }
    // kiểm tra trùng ID
    for (const auto& p : pitches) {
        if (p.getId() == id) {
            std::cout << "ID san da ton tai!\n";
            return;
        }
    }
    pitches.emplace_back(id, name,price,size);
    std::cout << "Them san thanh cong!\n";
}

void AdminController::updatePitch(std::vector<Pitch>& pitches) {
    int id;
    std::cout << "Nhap ID san muon sua: ";
    std::cin >> id;
    std::cin.ignore();

    for (auto& p : pitches) {
        if (p.getId() == id) {
            std::string newName;
            std::cout << "Nhap ten moi: ";
            std::getline(std::cin, newName);
            p.setName(newName);
            std::cout << "Cap nhat thanh cong!\n";
            return;
        }
    }
    std::cout << "Khong tim thay san.\n";
}

void AdminController::deletePitch(std::vector<Pitch>& pitches) {
    int id;
    std::cout << "Nhap ID san muon xoa: ";
    std::cin >> id;

    for (auto it = pitches.begin(); it != pitches.end(); ++it) {
        if (it->getId() == id) {
            pitches.erase(it);
            std::cout << "Xoa san thanh cong!\n";
            return;
        }
    }
    std::cout << "Khong tim thay san.\n";
}

// ================== HÀM MỚI Ở DƯỚI NÀY ==================

void AdminController::bookPitchOffline(const std::vector<Pitch>& pitches,
                                       std::vector<Booking>& bookings) {
    if (pitches.empty()) {
        std::cout << "Chua co san nao trong he thong.\n";
        return;
    }

    int id;
    std::cout << "\n=== DAT SAN OFFLINE TAI QUAY ===\n";
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

    // Xóa \n còn trong buffer trước khi dùng getline
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string customerName;
    std::cout << "Nhap ten khach (co the bo trong neu khong can luu ten): ";
    std::getline(std::cin, customerName);
    if (customerName.empty()) {
        customerName = "OFFLINE";
    }

    std::string timeSlot;
    std::cout << "Nhap khung gio (VD: 2025-12-01 18:00-20:00)\n";
    std::cout << "Trong do thoi gian KET THUC la gio tra san: ";
    std::getline(std::cin, timeSlot);

    // Kiểm tra trùng lịch
    for (const auto& b : bookings) {
        if (b.getPitchId() == id && b.getTimeSlot() == timeSlot) {
            std::cout << "Khung gio nay cho san nay da co lich dat.\n";
            return;
        }
    }

    // Tạo booking mới (username = tên khách hoặc OFFLINE)
    bookings.emplace_back(id, customerName, timeSlot);

    std::cout << "Dat san OFFLINE thanh cong cho khach: " << customerName
              << " | San: " << selectedPitch->getName()
              << " | Khung gio: " << timeSlot << "\n";
}