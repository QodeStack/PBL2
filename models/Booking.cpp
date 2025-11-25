#include "Booking.h"

int Booking::nextId = 1;
// tạo mới 
Booking::Booking(int pitchId, const std::string& customerUsername, const std::string& timeSlot)
    : id(nextId++),
      pitchId(pitchId),
      customerUsername(customerUsername),
      timeSlot(timeSlot) {}
    
// load từ file (id đã biết)
Booking::Booking(int id, int pitchId, const std::string& customerUsername, const std::string& timeSlot)
    : id(id),
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
// set nextId
void Booking::setNextId(int value) {
    nextId = value;
}
