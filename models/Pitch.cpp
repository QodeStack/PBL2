#include "Pitch.h"

Pitch::Pitch(int id, const std::string& name, double price, int size)
    : id(id), name(name),price(price),size(size) {}


// 4 hàm xử lí lấy dữ liệu cho chức năng xem danh sách sân
int Pitch::getId() const {
    return id;
}
std::string Pitch::getName() const {
    return name;
}
double Pitch::getPrice() const { 
    return price;
}
int Pitch::getSize() const {      
    return size;
}


// 3 hàm xử lí thay đổi dữ liệu cho chức năng cập nhật 
void Pitch::setName(const std::string& newName) {
    name = newName;
}
void Pitch::setPrice(double newPrice) {
    price = newPrice;
}
void Pitch::setSize(int newSize) {
    size = newSize;
}
