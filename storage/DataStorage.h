#ifndef DATA_STORAGE_H
#define DATA_STORAGE_H

#include <vector>
#include <string>
#include "../models/Pitch.h"
#include "../models/Booking.h"

// Pitches
void loadPitchesFromFile(std::vector<Pitch>& pitches, const std::string& filename);
void savePitchesToFile(const std::vector<Pitch>& pitches, const std::string& filename);

// Bookings
void loadBookingsFromFile(std::vector<Booking>& bookings, const std::string& filename);
void saveBookingsToFile(const std::vector<Booking>& bookings, const std::string& filename);

#endif // DATA_STORAGE_H
