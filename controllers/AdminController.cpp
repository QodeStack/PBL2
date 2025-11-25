#include "AdminController.h"
#include <iostream>

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
