#include "Booking.h"

int Booking::nextId = 1;
Booking::Booking(int id,
                 int pitchId,
                 const std::string& username,
                 const std::string& timeSlot,
                 const std::string& startTime,
                 const std::string& endTime,
                 BookingStatus status,
                 double totalAmount)
    : id(id),
      pitchId(pitchId),
      customerUsername(username),
      timeSlot(timeSlot),
      startTime(startTime),
      endTime(endTime),
      status(status),
      totalAmount(totalAmount){}

Booking::Booking(int pitchId,
                 const std::string& username,
                 const std::string& timeSlot,
                 const std::string& startTime,
                 const std::string& endTime,
                 BookingStatus status,
                 double totalAmount)
    : id(nextId++),
      pitchId(pitchId),
      customerUsername(username),
      timeSlot(timeSlot),
      startTime(startTime),
      endTime(endTime),
      status(status),
      totalAmount(totalAmount){}


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
/////// Mới ///////
std::string Booking::getStartTime() const {
    return startTime;
}

std::string Booking::getEndTime() const {
    return endTime;
}

BookingStatus Booking::getStatus() const {
    return status;
}

double Booking::getTotalAmount() const {
    return totalAmount;
}

void Booking::setStartTime(const std::string& s) {
    startTime = s;
}

void Booking::setEndTime(const std::string& s) {
    endTime = s;
}

void Booking::setStatus(BookingStatus st) {
    status = st;
}

void Booking::setTotalAmount(double amount) {
    totalAmount = amount;
}
///// Mới ////


// set nextId
void Booking::setNextId(int value) {
    nextId = value;
}
