#ifndef BOOKING_H
#define BOOKING_H

#include <string>

enum class BookingStatus { // Trạng thái booking 
    Active,    // Đang chơi / chưa tính tiền
    Finished  // Đã tính tiền xong
    
};
class Booking {
private:
    static int nextId;         // biến static để tự tăng id
    int id;                    // ID của booking (tự tăng)
    int pitchId;               // ID sân
    std::string customerUsername; // username khách
    std::string timeSlot;      // Khung giờ, ví dụ: "2025-12-01 18:00-19:00"
    std::string startTime;  // "YYYY-MM-DD HH:MM" - thời gian bắt đầu thực tế
    std::string endTime;    // "YYYY-MM-DD HH:MM" - thời gian kết thúc thực tế (offline)
    BookingStatus status;   // Active / Finished
    double totalAmount;     // Số tiền đã tính (sau khi checkout)
    
public:
     // Dùng khi load từ file với ID đã có
    Booking(int id,int pitchId,const std::string& username,const std::string& timeSlot,
            const std::string& startTime = "",const std::string& endTime   = "",
            BookingStatus status = BookingStatus::Active,double totalAmount = 0.0);

     // Dùng khi tạo mới booking trong chương trình (ID auto tăng)
    Booking(int pitchId,const std::string& username,const std::string& timeSlot, const std::string& startTime = "",
            const std::string& endTime   = "",BookingStatus status = BookingStatus::Active, double totalAmount = 0.0);


    int getId() const; // trả về ID của booking trong danh sách đặt sân 
    int getPitchId() const; // trả về ID của sân trong danh sách sân 
    std::string getCustomerUsername() const; //trả về tên khách hàng đã đặt sân 
    std::string getTimeSlot() const;  // Khung giờ [giờ bắt đầu -giờ kết thúc ]: sử dụng cho đặt sân online 
    std::string getStartTime() const; // lấy ra giờ bắt đầu thực tế (lấy thời gian hiện tại của máy tính ) : sử dụng cho đặt sân offline   
    std::string getEndTime() const;  // lấy ra giờ kết thúc thức tế (lấy thời gian hiện tại của máy tính) : sử dụng cho đặt sân offline   
    BookingStatus getStatus() const; // trả về trạng thái sân đang bị chiếm hay sân đã tính tiền (Active/Finished)
    double getTotalAmount() const;// trả về tổng tiền đã chơi cho booking này 

    void setStartTime(const std::string& s); // thiết lập thời gian bắt đầu thực tế -> dùng cho đặt sân tại quầy 
    void setEndTime(const std::string& s); // thiệt lập thời gian kết thúc thực tế -> dùng cho đặt sân tại quầy 
    void setStatus(BookingStatus st); // đổi trạng thái booking : khi tạo booking thì chuyển sang active , còn khi tính tiền xong thì chuyển sang Fininshed
    void setTotalAmount(double amount); //Gán tổng tiền cho booking.
    static void setNextId(int value); // set ID tăng trong booking
};

#endif // BOOKING_H
