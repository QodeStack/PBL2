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

        if (!std::getline(ss, idStr, ';')) continue;
        if (!std::getline(ss, pitchIdStr, ';')) continue;
        if (!std::getline(ss, username, ';')) continue;
        if (!std::getline(ss, timeSlot, ';')) continue;

        int id = std::stoi(idStr);
        int pitchId = std::stoi(pitchIdStr);

        bookings.emplace_back(id, pitchId, username, timeSlot);

        if (id > maxId) maxId = id;
    }

    inFile.close();

    // Đặt nextId = maxId + 1 để booking mới không bị trùng ID
    Booking::setNextId(maxId + 1);
}

void saveBookingsToFile(const std::vector<Booking>& bookings, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cout << "Khong mo duoc file de ghi: " << filename << "\n";
        return;
    }

    for (const auto& b : bookings) {
        outFile << b.getId() << ";"
                << b.getPitchId() << ";"
                << b.getCustomerUsername() << ";"
                << b.getTimeSlot()
                << "\n";
    }

    outFile.close();
}
