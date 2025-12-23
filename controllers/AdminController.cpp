#include "AdminController.h"
#include <iostream>
#include <limits>
#include "../helpers/DateTimeHelper.h"

// Xử lí : việc tạo sân ( kiểm tra logic )

bool AdminController::createPitch(std::vector<Pitch> &pitches,int id, const std::string &name,double price, int size,std::string &outMsg)
{
    if (size != 5 && size != 7 && size != 11)
    {
        outMsg = "Loai san khong hop le! Chi chap nhan 5, 7, 11.";
        return false;
    }
    for (const auto &p : pitches)
    {
        if (p.getId() == id)
        {
            outMsg = "ID san da ton tai!";
            return false;
        }
    }
    // Nếu không bị sai 2 điều kiện trên thì thêm sân
    pitches.emplace_back(id, name, price, size);
    outMsg = "Them san thanh cong!";
    return true;
}

bool AdminController::updatePitch(std::vector<Pitch> &pitches,int id,const std::string &newName,double newPrice,int newSize,std::string &outMsg)
{
    if (pitches.empty())
    {
        outMsg = "Chua co san nao de sua.";
        return false;
    }
    for (auto &p : pitches)
    {
        if (p.getId() != id)
            continue;

        std::ostringstream oss;

        // Thông tin hiện tại (giống bản cũ)
        oss << "Thong tin hien tai:\n";
        oss << "ID: " << p.getId()
            << " | Ten: " << p.getName()
            << " | Gia: " << p.getPrice()
            << " | Loai san: " << p.getSize() << " nguoi\n\n";

        // ====== SỬA TÊN SÂN ======
        if (!newName.empty())
        {
            p.setName(newName);
        }

        // ====== SỬA GIÁ SÂN ======
        // Quy ước từ logic cũ: -1 => giữ nguyên; >=0 => set; khác -1 mà “không hợp lệ” => báo giữ nguyên
        // Với UI mới thì newPrice luôn là số (không bị cin.fail), nên “không hợp lệ” thực tế là < -1
        if (newPrice >= 0)
        {
            p.setPrice(newPrice);
        }
        else if (newPrice != -1)
        {
            oss << "Gia khong hop le, giu nguyen gia cu.\n";
        }

        // ====== SỬA LOẠI SÂN ======
        // Quy ước từ logic cũ: -1 => giữ nguyên; 5/7/11 => set; khác -1 => báo giữ nguyên
        if (newSize == 5 || newSize == 7 || newSize == 11)
        {
            p.setSize(newSize);
        }
        else if (newSize != -1)
        {
            oss << "Loai san khong hop le, giu nguyen loai san cu.\n";
        }
        // In thông tin mới (giống bản cũ)
        oss << "\nCap nhat thanh cong!\n";
        oss << "Thong tin moi:\n";
        oss << "ID: " << p.getId()
            << " | Ten: " << p.getName()
            << " | Gia: " << p.getPrice()
            << " | Loai san: " << p.getSize() << " nguoi\n";

        outMsg = oss.str();
        return true;
    }
    outMsg = "Khong tim thay san.";
    return false;
}

bool AdminController::deletePitchById(std::vector<Pitch> &pitches, int id)
{
    for (auto it = pitches.begin(); it != pitches.end(); ++it)
    {
        if (it->getId() == id)
        {
            pitches.erase(it);
            return true; 
        }
    }
    return false; 
}

bool AdminController::bookPitchOffline(const std::vector<Pitch> &pitches,std::vector<Booking> &bookings,int pitchId,const std::string &customerNameInput,std::string &outMsg)
{
    if (pitches.empty())
    {
        outMsg = "Chua co san nao trong he thong.";
        return false;
    }
    // tìm sân
    const Pitch *selectedPitch = nullptr;
    for (const auto &p : pitches)
    {
        if (p.getId() == pitchId)
        {
            selectedPitch = &p;
            break;
        }
    }

    if (!selectedPitch)
    {
        outMsg = "Khong tim thay san.";
        return false;
    }

    // tên khách
    std::string customerName = customerNameInput;
    if (customerName.empty())
        customerName = "OFFLINE";
    // start time hiện tại
    std::string start = DateTimeHelper::getCurrentTimeString();
    // tạo booking offline (timeSlot rỗng, endTime rỗng, Active, total=0)
    bookings.emplace_back(
        pitchId,customerName,          
        "",                    // timeSlot (offline)
        start,                 // startTime
        "",                    // endTime
        BookingStatus::Active, // status
        0.0                    // total
    );

    std::ostringstream oss;
    oss << "Dat san OFFLINE thanh cong cho khach: " << customerName
        << " | San: " << selectedPitch->getName()
        << " | Bat dau luc: " << start;

    outMsg = oss.str();
    return true;
}


bool AdminController::getCheckoutCandidates(const std::vector<Pitch>& pitches,const std::vector<Booking>& bookings,int pitchId,std::vector<std::string>& outLines,
                                            std::string& outErr) const
{
    // 1) tìm sân
    const Pitch* pitchPtr = nullptr;
    for (const auto& p : pitches) {
        if (p.getId() == pitchId) { pitchPtr = &p; break; }
    }
    if (!pitchPtr) {
        outErr = "Khong tim thay san.";
        return false;
    }

    // 2) lọc candidateBookings (y hệt code cũ)
    outLines.clear();
    for (const auto& b : bookings) {
        if (b.getPitchId() == pitchId &&
            b.getStatus() == BookingStatus::Active &&
            b.getTotalAmount() == 0.0)
        {
            std::ostringstream oss;
            oss << "- Booking ID: " << b.getId();

            if (!b.getTimeSlot().empty()) {
                oss << " | Kieu: ONLINE | TimeSlot: " << b.getTimeSlot();
            } else {
                oss << " | Kieu: OFFLINE | Bat dau: " << b.getStartTime();
            }

            oss << " | Khach: " << b.getCustomerUsername()
                << " | Tong tien hien tai: " << b.getTotalAmount();

            outLines.push_back(oss.str());
        }
    }

    if (outLines.empty()) {
        outErr = "Khong co booking nao dang hoat dong cho san nay.";
        return false;
    }

    return true;
}

bool AdminController::checkoutPitch(std::vector<Pitch>& pitches,std::vector<Booking>& bookings,int pitchId,int bookingId,std::string& outBillMsg)
{
    // 1) tìm sân
    Pitch* pitchPtr = nullptr;
    for (auto& p : pitches) {
        if (p.getId() == pitchId) { pitchPtr = &p; break; }
    }
    if (!pitchPtr) { outBillMsg = "Khong tim thay san."; return false; }

    // 2) tìm booking hợp lệ (đúng điều kiện cũ)
    Booking* target = nullptr;
    for (auto& b : bookings) {
        if (b.getId() == bookingId &&
            b.getPitchId() == pitchId &&
            b.getStatus() == BookingStatus::Active &&
            b.getTotalAmount() == 0.0)
        {
            target = &b; break;
        }
    }
    if (!target) { outBillMsg = "Booking ID khong hop le."; return false; }

    double hours = 0.0;
    double amount = 0.0;

    // 3) tính tiền Online/Offline (y hệt code cũ)
    if (!target->getTimeSlot().empty()) {
        hours = DateTimeHelper::hoursFromTimeSlot(target->getTimeSlot());
        if (hours <= 0) { outBillMsg = "Khong the tinh duoc thoi gian tu timeSlot."; return false; }
        amount = hours * pitchPtr->getPrice();
    } else {
        std::string start = target->getStartTime();
        std::string end = DateTimeHelper::getCurrentTimeString();
        if (start.empty()) { outBillMsg = "Booking offline khong co startTime hop le."; return false; }

        hours = DateTimeHelper::diffHours(start, end);
        if (hours <= 0) { outBillMsg = "Thoi gian choi <= 0, kiem tra lai."; return false; }

        amount = hours * pitchPtr->getPrice();
        target->setEndTime(end);
    }
    // 4) update (y hệt code cũ)
    target->setTotalAmount(amount);
    target->setStatus(BookingStatus::Finished);
    // 5) bill (nhiều dòng)
    std::ostringstream oss;
    oss << "=== HOA DON DON GIAN ===\n";
    oss << "San: " << pitchPtr->getName() << " (ID: " << pitchId << ")\n";
    oss << "Booking ID: " << target->getId() << "\n";
    oss << "Khach: " << target->getCustomerUsername() << "\n";
    oss << "So gio tinh tien: " << hours << "\n";
    oss << "Don gia/1 gio: " << pitchPtr->getPrice() << "\n";
    oss << "Tong tien: " << amount << "\n";

    outBillMsg = oss.str();
    return true;
}

