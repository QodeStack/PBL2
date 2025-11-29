#ifndef BOOKING_H
#define BOOKING_H

#include <string>

enum class BookingStatus {
    Active,    // Đang chơi / chưa tính tiền
    Finished   // Đã tí nh tiền xong
};
class Booking {
private:
    static int nextId;         // biến static để tự tăng id

    int id;                    // ID của booking (tự tăng)
    int pitchId;               // ID sân
    std::string customerUsername; // username khách
    std::string timeSlot;      // Khung giờ, ví dụ: "2025-12-01 18:00-19:00"
    // 🌟 MỚI:
    std::string startTime;  // "YYYY-MM-DD HH:MM" - thời gian bắt đầu thực tế
    std::string endTime;    // "YYYY-MM-DD HH:MM" - thời gian kết thúc thực tế (offline)
    BookingStatus status;   // Active / Finished
    double totalAmount;     // Số tiền đã tính (sau khi checkout)
    

public:
     // Dùng khi load từ file với ID đã có
    Booking(int id,
            int pitchId,
            const std::string& username,
            const std::string& timeSlot,
            const std::string& startTime = "",
            const std::string& endTime   = "",
            BookingStatus status = BookingStatus::Active,
            double totalAmount = 0.0);

     // Dùng khi tạo mới booking trong chương trình (ID auto tăng)
    Booking(int pitchId,
            const std::string& username,
            const std::string& timeSlot,
            const std::string& startTime = "",
            const std::string& endTime   = "",
            BookingStatus status = BookingStatus::Active,
            double totalAmount = 0.0);

    int getId() const;
    int getPitchId() const;
    std::string getCustomerUsername() const;
    std::string getTimeSlot() const;
    
    std::string getStartTime() const;
    std::string getEndTime() const;
    BookingStatus getStatus() const;
    double getTotalAmount() const;

    void setStartTime(const std::string& s);
    void setEndTime(const std::string& s);
    void setStatus(BookingStatus st);
    void setTotalAmount(double amount);


    // 🌟 Cho phép set lại nextId sau khi load file
    static void setNextId(int value);
};

#endif // BOOKING_H
