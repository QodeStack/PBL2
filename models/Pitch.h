#ifndef PITCH_H
#define PITCH_H

#include <string>

class Pitch {
private:
    int id; // mã sân 
    std::string name; // tên sân 
    double price; // giá sân 
    int size; // loại sân 
public:
    Pitch(int id, const std::string& name,double price, int size);

    int getId() const;
    std::string getName() const;
    double getPrice() const; 
    int getSize() const; 

    void setPrice(double newPrice);
    void setSize(int newSize); 
    void setName(const std::string& newName);

};

#endif // PITCH_H
