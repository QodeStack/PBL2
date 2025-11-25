#ifndef PITCH_H
#define PITCH_H

#include <string>

class Pitch {
private:
    int id;
    std::string name;
    bool isBooked;
    std::string bookedBy; // username khách đặt
    double price;
    int size; 
    std::string bookedTime; 
public:
    Pitch(int id, const std::string& name,double price, int size);

    int getId() const;
    std::string getName() const;
    bool getIsBooked() const;
    std::string getBookedBy() const;

    double getPrice() const; 
    int getSize() const; 

    std::string getBookedTime() const;  

    void setName(const std::string& newName);
    void setBooked(bool booked);
    void setBookedBy(const std::string& username);
    void setPrice(double newPrice);
    void setSize(int newSize); 
    
    void setBookedTime(const std::string& time);
};

#endif // PITCH_H
