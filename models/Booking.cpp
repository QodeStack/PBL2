#include "Booking.h"

int Booking::nextId = 1;

Booking::Booking(int pitchId, const std::string& customerUsername, const std::string& timeSlot)
    : id(nextId++),
      pitchId(pitchId),
      customerUsername(customerUsername),
      timeSlot(timeSlot) {}

int Booking::getId() const {
    return id;
}

int Booking::getPitchId() const {
    return pitchId;
}

std::string Booking::getCustomerUsername() const {
    return customerUsername;
}

std::string Booking::getTimeSlot() const {
    return timeSlot;
}
