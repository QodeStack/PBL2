#ifndef ADMIN_CONTROLLER_H
#define ADMIN_CONTROLLER_H

#include <vector> 
#include "../models/Pitch.h"
#include "../models/Booking.h"

class AdminController {
public:
    
    bool createPitch(std::vector<Pitch>& pitches,
                 int id, const std::string& name, double price, int size,
                 std::string& outMsg);

    bool updatePitch(std::vector<Pitch>& pitches, int id,
                 const std::string& newName, double newPrice, int newSize,
                 std::string& outMsg);

    bool deletePitchById(std::vector<Pitch>& pitches, int id);
   
    bool bookPitchOffline(const std::vector<Pitch>& pitches,
                      std::vector<Booking>& bookings,
                      int pitchId,
                      const std::string& customerNameInput,
                      std::string& outMsg);

    bool getCheckoutCandidates(const std::vector<Pitch>& pitches,
                           const std::vector<Booking>& bookings,
                           int pitchId,
                           std::vector<std::string>& outLines,
                           std::string& outErr) const;
    bool checkoutPitch(std::vector<Pitch>& pitches,
                   std::vector<Booking>& bookings,
                   int pitchId,
                   int bookingId,
                   std::string& outBillMsg);



};

#endif // ADMIN_CONTROLLER_H
