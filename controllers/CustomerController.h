#ifndef CUSTOMER_CONTROLLER_H
#define CUSTOMER_CONTROLLER_H

#include <vector>
#include <string>
#include "../models/Pitch.h"
#include "../models/Booking.h"

class CustomerController {
public:
    void viewAllPitches(const std::vector<Pitch>& pitches) const;

    // Xem sân trống theo 1 khung giờ
    void viewFreePitches(const std::vector<Pitch>& pitches,
                         const std::vector<Booking>& bookings) const;

    // Đặt sân: chọn pitch + khung giờ, lưu vào bookings
    void bookPitch(const std::vector<Pitch>& pitches,
                   std::vector<Booking>& bookings,
                   const std::string& username);
};

#endif // CUSTOMER_CONTROLLER_H
