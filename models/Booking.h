#ifndef BOOKING_H
#define BOOKING_H

#include <string>

class Booking {
private:
    int id;                    // ID của booking (tự tăng)
    int pitchId;               // ID sân
    std::string customerUsername; // username khách
    std::string timeSlot;      // Khung giờ, ví dụ: "2025-12-01 18:00-19:00"

    static int nextId;         // biến static để tự tăng id

public:
    // Dùng khi tạo booking mới trong chương trình
    Booking(int pitchId, const std::string& customerUsername, const std::string& timeSlot);

    // 🌟 Dùng khi load từ file (id có sẵn)
    Booking(int id, int pitchId, const std::string& customerUsername, const std::string& timeSlot);

    int getId() const;
    int getPitchId() const;
    std::string getCustomerUsername() const;
    std::string getTimeSlot() const;

    // 🌟 Cho phép set lại nextId sau khi load file
    static void setNextId(int value);
};

#endif // BOOKING_H
