#include "Pitch.h"

Pitch::Pitch(int id, const std::string& name, double price, int size)
    : id(id), name(name), isBooked(false), bookedBy(""),price(price),size(size),bookedTime("") {}

int Pitch::getId() const {
    return id;
}

std::string Pitch::getName() const {
    return name;
}

bool Pitch::getIsBooked() const {
    return isBooked;
}

std::string Pitch::getBookedBy() const {
    return bookedBy;
}

double Pitch::getPrice() const {  // ← phải có const giống trong .h
    return price;
}
int Pitch::getSize() const {      // ← phải có const giống trong .h
    return size;
}
std::string Pitch::getBookedTime() const {   // 🌟 implement getter
    return bookedTime;
}

void Pitch::setName(const std::string& newName) {
    name = newName;
}

void Pitch::setBooked(bool booked) {
    isBooked = booked;
}

void Pitch::setBookedBy(const std::string& username) {
    bookedBy = username;
}

void Pitch::setPrice(double newPrice) {
    price = newPrice;
}

void Pitch::setSize(int newSize) {
    size = newSize;
}
void Pitch::setBookedTime(const std::string& time) {  // 🌟 implement setter
    bookedTime = time;
}
