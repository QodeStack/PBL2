#include "DataStorage.h"
#include <fstream>
#include <sstream>
#include <iostream>

// ---------------- PITCHES ----------------

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

// ---------------- BOOKINGS ----------------

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
