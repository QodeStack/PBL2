#include "MenuView.h"
#include <iostream>

int MenuView::showMainMenu() const {
    int choice;
    std::cout << "\n===== HE THONG QUAN LY SAN BONG =====\n";
    std::cout << "1. Dang nhap\n";
    std::cout << "2. Dang ky (khach hang)\n";
    std::cout << "0. Thoat\n";
    std::cout << "Chon: ";
    std::cin >> choice;
    return choice;
}

int MenuView::showAdminMenu() const {
    int choice;
    std::cout << "\n===== MENU ADMIN =====\n";
    std::cout << "1. Xem danh sach san\n";
    std::cout << "2. Them san (Create)\n";
    std::cout << "3. Sua san (Update)\n";
    std::cout << "4. Xoa san (Delete)\n";
    std::cout << "5. Dat san OFFLINE tai quay\n";   // <-- THÊM DÒNG NÀY
    std::cout << "6. Tinh tien san\n";
    std::cout << "0. Dang xuat\n";
    std::cout << "Chon: ";
    std::cin >> choice;
    return choice;
}

int MenuView::showCustomerMenu() const {
    int choice;
    std::cout << "\n===== MENU KHACH HANG =====\n";
    std::cout << "1. Xem tat ca san\n";
    std::cout << "2. Xem san trong\n";
    std::cout << "3. Xem san da dat\n";
    std::cout << "4. Dat san\n";
    std::cout << "0. Dang xuat\n";
    std::cout << "Chon: ";
    std::cin >> choice;
    return choice;
}
