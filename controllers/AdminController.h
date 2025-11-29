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
    // 🌟 HÀM MỚI: Admin đặt sân offline tại quầy
    void bookPitchOffline(const std::vector<Pitch>& pitches,
                          std::vector<Booking>& bookings);
    void checkoutPitch(std::vector<Pitch>& pitches,
                   std::vector<Booking>& bookings);
};

#endif // ADMIN_CONTROLLER_H
