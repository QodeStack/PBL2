#ifndef ADMIN_CONTROLLER_H
#define ADMIN_CONTROLLER_H

#include <vector>
#include "../models/Pitch.h"
#include "../models/Booking.h"   // <-- THÊM DÒNG NÀY

class AdminController {
public:
    void listPitches(const std::vector<Pitch>& pitches) const;
    void createPitch(std::vector<Pitch>& pitches);
    void updatePitch(std::vector<Pitch>& pitches);
    void deletePitch(std::vector<Pitch>& pitches);
    
    // Admin đặt sân offline tại quầy
    void bookPitchOffline(const std::vector<Pitch>& pitches,
                          std::vector<Booking>& bookings);
    void checkoutPitch(std::vector<Pitch>& pitches,
                   std::vector<Booking>& bookings);
    
    // ✅ THÊM HÀM MỚI:
    // Xem các lịch đặt CHƯA tính tiền
    void viewUnpaidBookings(const std::vector<Booking>& bookings,
                            const std::vector<Pitch>& pitches) const;
                            
    void createPitch(std::vector<Pitch>& pitches, int id, const std::string& name, double price, int size);
    bool updatePitch(std::vector<Pitch>& pitches, int id, const std::string& newName, double newPrice, int newSize);
    bool deletePitchById(std::vector<Pitch>& pitches, int id);
    bool bookPitchOffline(const std::vector<Pitch>& pitches,std::vector<Booking>& bookings,int pitchId,const std::string& customerNameInput,std::string& outMsg);




};

#endif // ADMIN_CONTROLLER_H
