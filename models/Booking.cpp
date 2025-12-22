#include "Booking.h"

int Booking::nextId = 1;
// Dữ liệu booking được load lên từ file txt
Booking::Booking(int id, int pitchId, const std::string& username, const std::string& timeSlot, const std::string& startTime,
                 const std::string& endTime, BookingStatus status, double totalAmount)
    : id(id), pitchId(pitchId), customerUsername(username), timeSlot(timeSlot), startTime(startTime),
      endTime(endTime), status(status), totalAmount(totalAmount){}

// Dữ liệu booking được tạo khi customer đặt sân hoặc admin đặt sân tại quầy 
Booking::Booking(int pitchId,const std::string& username,const std::string& timeSlot,const std::string& startTime,
                 const std::string& endTime,BookingStatus status,double totalAmount)
    : id(nextId++),pitchId(pitchId),customerUsername(username),timeSlot(timeSlot), startTime(startTime),
      endTime(endTime),status(status),totalAmount(totalAmount){}


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

void Booking::setNextId(int value) {
    nextId = value;
}
