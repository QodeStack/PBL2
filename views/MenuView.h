#include <string>  // ✅ để dùng std::string
#include <utility> // ✅ để dùng std::pair
#ifndef MENU_VIEW_H
#define MENU_VIEW_H
#include <vector>            
#include "../models/Pitch.h" 
#include "../models/Booking.h"
#include "TerminalUI.h"
struct PitchFormInput
{
    int id;
    std::string name;
    double price;
    int size;
};
struct UpdatePitchInput
{
    int id;
    std::string newName;
    double newPrice;
    int newSize;
};
struct OfflineBookingInput
{
    int pitchId;
    std::string customerName;
};
class MenuView
{
public:
    int showMainMenu() const;
    int showAdminMenu() const;
    int showCustomerMenu() const;

    std::pair<std::string, std::string> showLoginForm() const;
    std::pair<std::string, std::string> showRegisterForm();

    void pause() const;

    void showPitchesScreen(const std::vector<Pitch> &pitches) const;

    PitchFormInput showCreatePitchForm() const;

    UpdatePitchInput showUpdatePitchForm() const;

    std::pair<int, bool> showDeletePitchForm() const;

    OfflineBookingInput showOfflineBookingForm() const;
    //void showMessageBox(const std::string &title, const std::vector<std::string> &lines) const;
    void showMessageBox(const std::string& title,
                    const std::vector<std::string>& lines,
                    Color color = Color::Default) const;

    void showUnpaidBookingsScreen(const std::vector<Booking> &bookings,
                                  const std::vector<Pitch> &pitches) const;

    // tính tiềnA
    int showCheckoutChoosePitchForm() const;
    int showCheckoutChooseBookingForm() const;
};

#endif // MENU_VIEW_H
