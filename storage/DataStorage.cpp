#include "DataStorage.h"
#include <fstream>
#include <sstream>
#include <iostream>

//////////////////////////////// Pitches //////////////////////////////////////////

// load các sân đã được tạo từ file pitches.txt khi dự án bắt đầu được build 
void loadPitchesFromFile(std::vector<Pitch>& pitches, const std::string& filename) {
    pitches.clear();

    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        // Chưa có file -> lần đầu chạy, không sao
        return;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string idStr, name, priceStr, sizeStr;

        if (!std::getline(ss, idStr, ';')) continue;
        if (!std::getline(ss, name, ';')) continue;
        if (!std::getline(ss, priceStr, ';')) continue;
        if (!std::getline(ss, sizeStr, ';')) continue;

        int id = std::stoi(idStr);
        double price = std::stod(priceStr);
        int size = std::stoi(sizeStr);

        pitches.emplace_back(id, name, price, size);
    }

    inFile.close();
}

// lưu dữ liệu vào file pitches.txt
void savePitchesToFile(const std::vector<Pitch>& pitches, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cout << "Khong mo duoc file de ghi: " << filename << "\n";
        return;
    }

    for (const auto& p : pitches) {
        outFile << p.getId() << ";"
                << p.getName() << ";"
                << p.getPrice() << ";"
                << p.getSize()
                << "\n";
    }

    outFile.close();
}


//////////////////////////////// BOOKINGS //////////////////////////////////////////


// load các lịch đặt sân từ file bookings.txt khi dự án bắt đầu được build
void loadBookingsFromFile(std::vector<Booking>& bookings, const std::string& filename) {
    bookings.clear();

    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        // Chưa có file -> lần đầu chạy
        return;
    }

    std::string line;
    int maxId = 0;

    while (std::getline(inFile, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string idStr, pitchIdStr, username, timeSlot;
        std::string startTime, endTime, statusStr, amountStr;

        // Đọc 4 trường cơ bản trước
        if (!std::getline(ss, idStr, ';')) continue;
        if (!std::getline(ss, pitchIdStr, ';')) continue;
        if (!std::getline(ss, username, ';')) continue;
        if (!std::getline(ss, timeSlot, ';')) continue;

        int id      = std::stoi(idStr);
        int pitchId = std::stoi(pitchIdStr);

        // Cố đọc thêm các trường mới (có thể không có nếu là format cũ)
        if (!std::getline(ss, startTime, ';')) startTime = "";
        if (!std::getline(ss, endTime, ';'))   endTime   = "";
        if (!std::getline(ss, statusStr, ';')) statusStr = "1"; // default Finished
        if (!std::getline(ss, amountStr, ';')) amountStr = "0";

        BookingStatus status = (statusStr == "0")
                               ? BookingStatus::Active
                               : BookingStatus::Finished;
        double amount = 0.0;
        try {
            amount = std::stod(amountStr);
        } catch (...) {
            amount = 0.0;
        }

        bookings.emplace_back(
            id,
            pitchId,
            username,
            timeSlot,
            startTime,
            endTime,
            status,
            amount
        );

        if (id > maxId) maxId = id;
    }

    inFile.close();

    Booking::setNextId(maxId + 1);
}

// lưu dữ liệu vào file bookings.txt
void saveBookingsToFile(const std::vector<Booking>& bookings, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cout << "Khong mo duoc file de ghi: " << filename << "\n";
        return;
    }

    for (const auto& b : bookings) {
        int statusInt = (b.getStatus() == BookingStatus::Active) ? 0 : 1;

        outFile << b.getId() << ";"
                << b.getPitchId() << ";"
                << b.getCustomerUsername() << ";"
                << b.getTimeSlot() << ";"
                << b.getStartTime() << ";"
                << b.getEndTime() << ";"
                << statusInt << ";"
                << b.getTotalAmount()
                << "\n";
    }

    outFile.close();
}

//////////////////////////////// Users //////////////////////////////////////////

void loadUsersFromFile(std::vector<std::shared_ptr<User>>& users, const std::string& filename)
{
    std::ifstream inFile(filename);
    if (!inFile.is_open()) return;

    std::string line;
    while (std::getline(inFile, line))
    {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string username, password;

        if (!std::getline(ss, username, ';')) continue;
        if (!std::getline(ss, password)) continue; // ✅ lấy hết phần còn lại tới cuối dòng

        // tránh trùng username
        bool exists = false;
        for (const auto& u : users)
        {
            if (u && u->getUsername() == username)
            {
                exists = true;
                break;
            }
        }
        if (exists) continue;

        // ✅ tạo Customer
        users.push_back(std::make_shared<Customer>(username, password));
    }
}

void saveUsersToFile(const std::vector<std::shared_ptr<User>>& users, const std::string& filename)
{
    std::ofstream outFile(filename, std::ios::trunc);
    if (!outFile.is_open())
    {
        std::cout << "Khong mo duoc file de ghi: " << filename << "\n";
        return;
    }

    for (const auto& u : users)
    {
        if (!u) continue;

        // chỉ lưu Customer (admin tạo mặc định)
        if (u->getRole() == Role::Customer)
        {
            outFile << u->getUsername() << ";"
                    << u->getPassword()
                    << "\n";
        }
    }
}