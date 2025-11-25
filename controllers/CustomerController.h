#ifndef CUSTOMER_CONTROLLER_H
#define CUSTOMER_CONTROLLER_H

#include <vector>
#include <string>
#include "../models/Pitch.h"

class CustomerController {
public:
    void viewAllPitches(const std::vector<Pitch>& pitches) const;
    void viewFreePitches(const std::vector<Pitch>& pitches) const;
    void viewBookedPitches(const std::vector<Pitch>& pitches) const;
    void bookPitch(std::vector<Pitch>& pitches, const std::string& username);
};

#endif // CUSTOMER_CONTROLLER_H
