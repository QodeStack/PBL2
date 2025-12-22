#ifndef ADMIN_CONTROLLER_H
#define ADMIN_CONTROLLER_H

#include <vector> 
#include "../models/Pitch.h"
#include "../models/Booking.h"

class AdminController {
public:
    // Tạo sân 
    bool createPitch(std::vector<Pitch>& pitches,int id, const std::string& name, double price, int size, std::string& outMsg);

    // Cập nhật sân 
    bool updatePitch(std::vector<Pitch>& pitches, int id,const std::string& newName, double newPrice, int newSize, std::string& outMsg);

    // Xóa sân 
    bool deletePitchById(std::vector<Pitch>& pitches, int id);
   
    //Đặt sân tại quầy 
    bool bookPitchOffline(const std::vector<Pitch>& pitches,std::vector<Booking>& bookings, int pitchId,
                        const std::string& customerNameInput,std::string& outMsg);
                        
    // danh sách đã đặt và đang chơi nhưng chưa tính tiền 
    bool getCheckoutCandidates(const std::vector<Pitch>& pitches,const std::vector<Booking>& bookings,int pitchId,
                           std::vector<std::string>& outLines,std::string& outErr) const;

    // tính tiền 
    bool checkoutPitch(std::vector<Pitch>& pitches,std::vector<Booking>& bookings,int pitchId, int bookingId,std::string& outBillMsg);



};

#endif // ADMIN_CONTROLLER_H
